/*
 * P30.cpp
 *
 *  Created on: Feb 9, 2021
 *      Author: DevoutPrayer
 */

#include "P30.h"
#include "usart.h"
#ifdef USE_FREERTOS_DELAY
extern "C"{
void vTaskDelay(const uint32_t);
}
#define P30_Delay(x) vTaskDelay(x)
#else
#define P30_Delay(x) HAL_Delay(x)
#endif

P30::P30(P30Device* device) : p30Device{ device } {
	// TODO Auto-generated constructor stub

}

P30::~P30() {
	// TODO Auto-generated destructor stub
}

uint8_t P30::initialize(uint16_t ping_interval_ms)
{
		while(p30Device->init())
		{
			P30_Delay(1000);
		}
		while(!request(PingEnumNamespace::PingMessageId::PING1D_DEVICE_ID, 1000))
		{
			P30_Delay(100);
		}
		HAL_Delay(100);
		while(!request(PingEnumNamespace::PingMessageId::PING1D_FIRMWARE_VERSION, 1000))
		{
			P30_Delay(100);
		}
		HAL_Delay(100);
		while(!request(PingEnumNamespace::PingMessageId::PING1D_VOLTAGE_5, 1000))
		{
			P30_Delay(100);
		}
		HAL_Delay(100);
		while(!request(PingEnumNamespace::PingMessageId::PING1D_PROCESSOR_TEMPERATURE, 1000))
		{
			P30_Delay(100);
		}
		P30_Delay(100);
		while(!setPingInterval(ping_interval_ms,1))
		{
			P30_Delay(100);
		}
		P30_Delay(100);
//		if(!setPingEnable())
//		{
//			P30_Delay(100);
//		}
//		if(!setModeAuto(1,1))
//		{
//			P30_Delay(100);
//		}
		while(!setContinuousStart())
		{
			P30_Delay(100);
		}
		return 0;
 }

bool P30::update()
{
		if(!request(PingEnumNamespace::PingMessageId::PING1D_DISTANCE_SIMPLE, 100))
				return false;
		return true;
}

bool P30::handleMessage(ping_message *pMsg)
{
		switch(pMsg->message_id())
		{
				case (int)PingEnumNamespace::PingMessageId::PING1D_DEVICE_ID:
				{
						ping1d_device_id msg(pMsg->msgData, pMsg->msgDataLength());
						_device_id = msg.device_id();
						return true;
				}
				case (int)PingEnumNamespace::PingMessageId::COMMON_ACK:
				{
						return true;
				}
				case (int)PingEnumNamespace::PingMessageId::PING1D_DISTANCE_SIMPLE:
				{
						ping1d_distance_simple msg(pMsg->msgData, pMsg->msgDataLength());
						_distance = msg.distance();
						_confidence = msg.confidence();
						return true;
				}
				case (int)PingEnumNamespace::PingMessageId::PING1D_FIRMWARE_VERSION:
				{
						ping1d_firmware_version msg(pMsg->msgData, pMsg->msgDataLength());
						_device_type = msg.device_type();
						_device_model = msg.device_model();
						_firmware_version_major = msg.firmware_version_major();
						_firmware_version_minor = msg.firmware_version_minor();
						return true;
				}
				case (int) PingEnumNamespace::PingMessageId::PING1D_VOLTAGE_5:
				{
						ping1d_voltage_5 msg(pMsg->msgData, pMsg->msgDataLength());
						_voltage_5 = msg.voltage_5();
						return true;
				}
				case (int) PingEnumNamespace::PingMessageId::PING1D_PROCESSOR_TEMPERATURE:
				{
						ping1d_processor_temperature msg(pMsg->msgData, pMsg->msgDataLength());
						_processor_temperature = msg.processor_temperature();
						return true;
				}
				case (int)PingEnumNamespace::PingMessageId::PING1D_PING_INTERVAL:
				{
						ping1d_ping_interval msg(pMsg->msgData, pMsg->msgDataLength());
						_ping_interval = msg.ping_interval();
						return true;
				}
				case (int)PingEnumNamespace::PingMessageId::PING1D_MODE_AUTO:
				{
						ping1d_mode_auto msg(pMsg->msgData, pMsg->msgDataLength());
						_mode_auto = msg.mode_auto();
						return true;
				}
				case (int)PingEnumNamespace::PingMessageId::COMMON_NACK:
				{
						return false;
				}
				default:
						return false;
		}
}

ping_message* P30::request(enum PingEnumNamespace::PingMessageId id,
                           uint16_t timeout_ms)
{
        ping_msg_empty msg;
        msg.set_id((uint16_t)id);
        msg.updateChecksum();
        write(msg.msgData, msg.msgDataLength());
        ping_message* pmsg = waitMessage(id, timeout_ms);
        if(pmsg)
        {
        	handleMessage(pmsg);
        	return pmsg;
        }
        return nullptr;
}

bool P30::setPingInterval(uint16_t ping_interval, bool verify)
{
		ping1d_set_ping_interval msg;
		msg.set_ping_interval(ping_interval);
		msg.updateChecksum();
		write(msg.msgData, msg.msgDataLength());
		waitMessage((enum PingEnumNamespace::PingMessageId) 1, 1000);
		if(!request(PingEnumNamespace::PingMessageId::PING1D_PING_INTERVAL, 1000))
		{
				return false;
		}

		if(verify && (_ping_interval != ping_interval))
		{
				return false;
		}
		return true;
}

bool P30::setModeAuto(uint8_t mode_auto, bool verify)
{
		ping1d_set_mode_auto msg;
		msg.set_mode_auto(mode_auto);
		msg.updateChecksum();
		write(msg.msgData, msg.msgDataLength());
		waitMessage((enum PingEnumNamespace::PingMessageId) 1, 100);
		if(!request(PingEnumNamespace::PingMessageId::PING1D_MODE_AUTO, 100))
		{
				return false;
		}

		if(verify && (_mode_auto != mode_auto))
		{
				return false;
		}
		return true;
}

bool P30::setPingEnable()
{
		ping1d_set_ping_enable msg;
		msg.set_ping_enabled(1);
		msg.updateChecksum();
		write(msg.msgData, msg.msgDataLength());
		if(waitMessage((enum PingEnumNamespace::PingMessageId) 1, 100))
		{
			return true;
		}
		return false;

}

bool P30::setContinuousStart()
{
		ping1d_continuous_start msg;
		msg.set_id(1211);
		msg.updateChecksum();
		write(msg.msgData, msg.msgDataLength());
		if(waitMessage((enum PingEnumNamespace::PingMessageId) 1, 1000))
		{
			return true;
		}
		return false;

}

size_t P30::write(uint8_t* data, uint16_t length)
{
		return p30Device->write(data,length);
}

uint16_t P30::readByte()
{
		return p30Device->readByte();
}

ping_message* P30::read()
{
		uint16_t byte;
		uint8_t res;
		byte = readByte();
		while(byte < 256)
		{
				res = pingParser.parseByte(byte);
				if(res == PingParser::NEW_MESSAGE)
				{
					return &pingParser.rxMessage;
				}
				byte = readByte();
		}
#ifdef PRINT_DEBUG_INFORMATION
		printf("----------ping praser error: %d\n", (int)pingParser.errors);
#endif
		return nullptr;
}

ping_message* P30::waitMessage(enum PingEnumNamespace::PingMessageId id, uint16_t timeout_ms)
{
		uint32_t tstart = p30Device->get_tick();
		while( p30Device->get_tick() < tstart + timeout_ms )
		{
				ping_message* pmsg = read();

				if(!pmsg)
				{
					P30_Delay(20);
					continue;
				}
				return pmsg;
		}
		return nullptr;
}

