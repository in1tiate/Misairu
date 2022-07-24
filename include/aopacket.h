#ifndef AOPACKET_H
#define AOPACKET_H

#include <QDebug>
#include <QString>
#include <QStringList>

class AOPacket {
public:
  AOPacket(QString p_packet_string);
  AOPacket(QString header, QStringList &p_contents) : m_header(header), m_contents(p_contents){}

  QString get_header() const { return m_header; }
  const QStringList &get_contents() const { return m_contents; }
  QString to_string(bool encoded = false);

  void net_encode();
  void net_decode();

  static void escape(QStringList &contents);
  static void unescape(QStringList &contents);
private:

  QString m_header;
  QStringList m_contents;
};

#endif // AOPACKET_H
