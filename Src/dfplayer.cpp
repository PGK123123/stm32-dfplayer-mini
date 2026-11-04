#include "dfplayer.h"

DFPlayer::DFPlayer(UART& uart) : uart_module(uart) {

}

void DFPlayer::sendCommand(Command cmd, uint16_t param) {
    uint8_t high_param = (uint8_t)(param >> 8);
    uint8_t low_param  = (uint8_t)(param & 0xFF);

    uint16_t sum = 0xFF + 0x06 + (uint8_t)cmd + 0x00 + high_param + low_param;
    uint16_t checksum = -sum;

    uint8_t packet[10] = {
        0x7E,
        0xFF,
        0x06,
        (uint8_t)cmd,
        0x00,
        high_param,
        low_param,
        (uint8_t)(checksum >> 8),
        (uint8_t)(checksum & 0xFF),
        0xEF
    };

    uart_module.send_buff(packet, sizeof(packet));
}

void DFPlayer::play(uint16_t track) {
    sendCommand(Command::PlayMp3Folder, track);
    m_isPlaying = true;
    m_hasStarted = true;
}

void DFPlayer::pause() {
    sendCommand(Command::Pause);
    m_isPlaying = false;
}

void DFPlayer::change_volume(uint8_t volume) {
    if (volume > 30) {
        volume = 30;
    }
    sendCommand(Command::SetVolume, volume);
}

void DFPlayer::next_track() {
    sendCommand(Command::Next);
    m_isPlaying = true;
    m_hasStarted = true;
}

void DFPlayer::prev_track() {
    sendCommand(Command::Prev);
    m_isPlaying = true;
    m_hasStarted = true;
}

void DFPlayer::toggle() {
    if (m_isPlaying) {
        pause();
    } else {
        if (!m_hasStarted) {
            play(1);
        } else {
            sendCommand(Command::Resume);
            m_isPlaying = true;
        }
    }
}
