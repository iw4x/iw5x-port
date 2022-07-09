#include <std_include.hpp>
#include <loader/module_loader.hpp>
#include <utils/hook.hpp>

#include "game/game.hpp"

class security final : public module
{
public:
	void post_load() override
	{
		if (game::is_mp())
		{
			utils::hook(0x4AECD4, read_p2p_auth_ticket_stub, HOOK_JUMP).install()->quick();

			utils::hook(0x57680C, net_defer_packet_to_client, HOOK_CALL).install()->quick(); // SV_ConnectionlessPacket
		}
		else if (game::is_sp())
		{
			// Disable CL_ShellExecute_URL_f
			utils::hook::nop(0x4298F6, 5);
		}
	}

private:
	static void read_p2p_auth_ticket_stub(game::native::msg_t* msg, void* data, const int len)
	{
		if (len < 0) return;
		return game::native::MSG_ReadData(msg, data, std::min(len, 200));
	}

	static void net_defer_packet_to_client(game::native::netadr_s* net_from, game::native::msg_t* net_message)
	{
		assert(game::native::Sys_IsServerThread());

		if (static_cast<std::size_t>(net_message->cursize) >= sizeof(game::native::DeferredMsg::data))
		{
			return;
		}

		auto* msg = &game::native::deferredQueue->msgs[game::native::deferredQueue->send
			% std::extent_v<decltype(game::native::DeferredQueue::msgs)>];
		std::memcpy(msg->data, net_message->data, net_message->cursize);

		msg->datalen = net_message->cursize;
		msg->addr = *net_from;

		InterlockedIncrement(&game::native::deferredQueue->send);
	}
};

REGISTER_MODULE(security)
