/*
 * P30.h
 *
 *  Created on: Feb 9, 2021
 *      Author: DevoutPrayer
 */

#ifndef P30_P30_H_
#define P30_P30_H_

#include "ping-cpp/src/message/ping-message-all.h"
#include "ping-cpp/src/message/ping-parser.h"
#include "stdio.h"
#include "P30Device.h"


class P30 {
public:
		P30( P30Device* device);
		virtual ~P30();
		uint8_t initialize(uint16_t ping_interval_ms);
		bool update();
		bool setPingInterval(uint16_t ping_interval, bool verify);
		bool setModeAuto(uint8_t mode_auto, bool verify);
		bool setPingEnable();
		bool setContinuousStart();
		ping_message* waitMessage(enum PingEnumNamespace::PingMessageId id, uint16_t timeout_ms);
		ping_message* read();
		bool handleMessage(ping_message *pMsg);
        // Device type. 0: Unknown; 1: Echosounder
		uint8_t _device_type = 0;

		// Device model. 0: Unknown; 1: Ping1D
		uint8_t _device_model = 0;

		// Firmware major version.
		uint16_t _firmware_version_major = 0;

		// Firmware minor version.
		uint16_t _firmware_version_minor = 0;

		// The device ID (0-254). 255 is reserved for broadcast messages.
		uint8_t _device_id = 0;

		// Device supply voltage.
		uint16_t _voltage_5 = 0;

		// The speed of sound in the measurement medium. ~1,500,000 mm/s for water.
		uint32_t _speed_of_sound = 0;

		// The beginning of the scan region in mm from the transducer.
		uint32_t _scan_start = 0;

		// The length of the scan region.
		uint32_t _scan_length = 0;

		// The current operating mode of the device. 0: manual mode, 1: auto mode
		uint8_t _mode_auto = 0;

		// The interval between acoustic measurements.
		uint16_t _ping_interval = 0;

		// The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144
		uint32_t _gain_index = 0;

		// The acoustic pulse length during acoustic transmission/activation.
		uint16_t _pulse_duration = 0;

		// The current return distance determined for the most recent acoustic measurement.
		uint32_t _distance = 0;

		// Confidence in the most recent range measurement.
		uint16_t _confidence = 0;

		// The pulse/measurement count since boot.
		uint32_t _ping_number = 0;

		// The temperature in centi-degrees Centigrade (100 * degrees C).
		uint16_t _processor_temperature = 0;

		// The temperature in centi-degrees Centigrade (100 * degrees C).
		uint16_t _pcb_temperature = 0;

		// The state of the acoustic output. 0: disabled, 1:enabled
		uint8_t _ping_enabled = 0;

		// An array of return strength measurements taken at regular intervals across the scan region.
		uint16_t _profile_data_length = 0;
		uint8_t* _profile_data = 0;

		// The protocol version
		uint32_t _protocol_version = 0;
private:
        PingParser pingParser;
        P30Device* p30Device;
        size_t write(uint8_t* data, uint16_t length);
		uint16_t readByte();



		ping_message* request(enum PingEnumNamespace::PingMessageId id,
		                              uint16_t timeout_ms);


};

class ping_msg_empty : public ping_message
{
public:
        ping_msg_empty() : ping_message { 8 + 2 }
        {
                msgData[0] = 'B';
                msgData[1] = 'R';
                (uint16_t&)msgData[2] = 0; // payload size
                (uint16_t&)msgData[4] = 0; // ID
                msgData[6] = 0;
                msgData[7] = 0;
        }

        void set_id(const uint16_t id)
        { 
                memcpy((msgData + 4 + 0), &id, 2); 
        }

        uint16_t get_id()
        {
                if(msgData)
                        return *((uint16_t*)(msgData+4));
                else
                        return -1;
        }
};

#endif /* P30_P30_H_ */
