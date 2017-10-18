/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*
    This is a factory which produces protocols.

    By default, buildProtocol will create a protocol of the class given in
    self.protocol.
*/

#ifndef FACTORY_H
#define FACTORY_H

#include "protocol.h"
#include "connection.h"
#include "callback.h"

#include <map>
#include <set>
#include <iostream>
#include <boost/bind.hpp>

class Factory
{
private:
  int m_nNumProts;
  const Protocol *m_pProtocol;
  const std::string m_nProtocolName;
  std::map<Connection *, Protocol *> m_nProtocols;

public:
  Factory(const Protocol &prot);

  // void doStart();
  // void doStop();
  // void startFactory();
  // void stopFactory();

  ConnectionCallback connectCallback()
  {
    return ConnectionCallback(boost::bind(&Factory::createConnection,
                                          this, _1));
  }

  MessageCallback getMessageCallback()
  {
    return MessageCallback(boost::bind(&Factory::getMessage,
                                       this, _1, _2, _3));
  }

  CloseCallback closeConnectionCallback()
  {
    return CloseCallback(boost::bind(&Factory::lostConnection,
                                     this, _1));
  }

  void createConnection(Connection *newCon);
  void getMessage(Connection *con,
                  Buffer *buf,
                  Time time);
  void lostConnection(Connection *con);

  virtual void buildProtocol();
  // {
  //can be override
  // }
  ~Factory();
};

// class ServerFactory : public Factory
// {
// private:
// public:
//   ServerFactory(Protocol *);
//   ~ServerFactory();
// };

// class EchoServerFactory : public ServerFactory
// {
// private:
// public:
//   EchoServerFactory(Protocol *);
//   ~EchoServerFactory();
// };

// class WebServerFactory : public ServerFactory
// {
// private:
// public:
//   WebServerFactory(Protocol *);
//   ~WebServerFactory();
// };

//buildProtocol---return a Protocol
//statredConnecting
//clientConnectionLost
//clientConnectionFailed

#endif // FACTORY_H