/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: http://www.qt-project.org/
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**************************************************************************/
#include "shell.h"

#include <ssh/sshconnection.h>
#include <ssh/sshremoteprocess.h>

#include <QCoreApplication>
#include <QFile>
#include <QSocketNotifier>

#include <cstdlib>
#include <iostream>
#include<mainwindow.h>
using namespace QSsh;

Shell::Shell(const QSsh::SshConnectionParameters &parameters, QObject *parent)
    : QObject(parent),
      m_connection(new SshConnection(parameters)),no(i)
{
    ptr=(MainWindow*)parent;
    writeable=false;

    connect(m_connection,SIGNAL(connected()),SLOT(handleConnected()));
    connect(m_connection,SIGNAL(dataAvailable(QString)),this,SLOT(shellData(QString)));
    connect(this,SIGNAL(dataReady(int,QString)),ptr,SLOT(outToShell(int,QString)));
    connect(m_connection, SIGNAL(error(QSsh::SshError)),this,SLOT(shellError()));
    connect(this,SIGNAL(error(int,QString)),ptr,SLOT(outToShell(int,QString)));
    i++;
}

Shell::~Shell()
{
    delete m_connection;
}

void Shell::run()
{
    m_connection->connectToHost();
}

void Shell::handleConnected()
{

    m_shell=m_connection->createRemoteShell();
    m_shell->start();
    connect(m_shell.data(),SIGNAL(started()),this,SLOT(shellConnect()));
    connect(this,SIGNAL(connection(int,QString)),ptr,SLOT(outToShell(int,QString)));
    connect(m_shell.data(),SIGNAL(readyRead()),this,SLOT(shellOut()));
    connect(m_shell.data(), SIGNAL(closed(int)), this,SLOT(handleChannelClosed(int)));
}

void Shell::shellOut()
{
    ptr->outToShell(this->getNo(),m_shell->readAll()+"\n");
}
void Shell::handleChannelClosed(int exitStatus)
{

    qApp->exit(exitStatus == SshRemoteProcess::NormalExit && m_shell->exitCode() == 0
        ? EXIT_SUCCESS : EXIT_FAILURE);
}

void Shell::handleIn(QString &mse)
{
   if(writeable)
       m_shell->write("help\n\r");
}
void Shell::shellData(QString mse)
{
        emit dataReady(this->getNo(),mse);
}
void Shell::shellError()
{
    emit error(this->getNo(),this->m_connection->errorString());
}
void Shell::shellConnect()
{

    emit connection(this->getNo(),"成功连接");
    writeable=true;
}
