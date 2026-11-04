#pragma once

#include "uart.h"
#include "main.h"

enum class Command : uint8_t {
    Next        = 0x01,
    Prev        = 0x02,
    PlayTrack   = 0x03,
    VolumeUp    = 0x04,
    VolumeDown  = 0x05,
    SetVolume   = 0x06,
    Reset       = 0x0C,
    Resume      = 0x0D,
    Pause       = 0x0E,
    PlayMp3Folder = 0x12
};

class DFPlayer {
private:
	UART &uart_module;
	void sendCommand(Command cmd, uint16_t param = 0);
	bool m_isPlaying = false;
	bool m_hasStarted = false;

public:
	DFPlayer(UART& uart);

	void toggle();
	bool is_playing() const { return m_isPlaying; }

	void play(uint16_t track = 1);
	void pause();
	void change_volume(uint8_t volume);
	void next_track();
	void prev_track();
};
