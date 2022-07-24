#pragma once

#include "aopacket.h"
#include <optional>

class ICMessage {
public:
  ICMessage(){};

  /**
   * @brief Attempts to create an ICMessage based on an AOPacket.
   *
   * @param f_packet The packet to base the creation of the ICMessage on.
   *
   * @return If the packet was malformed, std::nullopt, else, an ICMessage.
   */
  static std::optional<ICMessage> from(const AOPacket &f_packet);

  AOPacket to_packet() const;

private:
  /// Represents the ID of the character who is "speaking" the message. There is
  /// technically a "-1" ID as well, but in AO language, a "-1" usually means
  /// "reset" or "nothing".
  quint16 m_charid;
};
