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
      m_connection(new SshConnection(parameters))
    , m_stdin(new QFile(this)),no(i)
{
    ptr=(MainWindow*)parent;
    connect(m_connection,SIGNAL(connected()),ptr,SLOT(ptr->outToShell(this->getNo(),tr("成功连接"))));
    connect(m_connection, SIGNAL(dataAvailable(QString &mse)),ptr,SLOT(ptr->outToShell(this->getNo(),tr(mse))));
    connect(m_connection, SIGNAL(error(QSsh::SshError)),ptr,SLOT(ptr->outToShell(this->getNo(),tr(m_connection->errorString()))));
    i++;
}

Shell::~Shell()
{
    delete m_connection;
}

void Shell::run()
{
    if (!m_stdin->open(stdin, QIODevice::ReadOnly | QIODevice::Unbuffered)) {
        std::cerr << "Error: Cannot read from standard input." << std::endl;
        qApp->exit(EXIT_FAILURE);
        return;
    }

    m_connection->connectToHost();
}

void Shell::handleConnectionError()
{
    std::cerr << "SSH connection error: " << qPrintable(m_connection->errorString()) << std::endl;
    qApp->exit(EXIT_FAILURE);
    //错误输出
}

void Shell::handleShellMessage(const QString &message)
{
    std::cout << qPrintable(message);
    //输出信息
}

void Shell::handleConnected()
{
    m_shell = m_connection->createRemoteShell();
    connect(m_shell.data(), SIGNAL(started()), SLOT(handleShellStarted()));
    connect(m_shell.data(), SIGNAL(readyReadStandardOutput()), SLOT(handleRemoteStdout()));
    connect(m_shell.data(), SIGNAL(readyReadStandardError()), SLOT(handleRemoteStderr()));
    connect(m_shell.data(), SIGNAL(closed(int)), SLOT(handleChannelClosed(int)));
    m_shell->start();
}

void Shell::handleShellStarted()
{
    QSocketNotifier * const notifier = new QSocketNotifier(0, QSocketNotifier::Read, this);
    connect(notifier, SIGNAL(activated(int)), SLOT(handleStdin()));
}

void Shell::handleRemoteStdout()
{
    std::cout << m_shell->readAllStandardOutput().data() << std::flush;
    //输出信息
}

void Shell::handleRemoteStderr()
{
    std::cerr << m_shell->readAllStandardError().data() << std::flush;
    //输出错误信息
}

void Shell::handleChannelClosed(int exitStatus)
{
    std::cerr << "Shell closed. Exit status was " << exitStatus << ", exit code was "
        << m_shell->exitCode() << "." << std::endl;
    qApp->exit(exitStatus == SshRemoteProcess::NormalExit && m_shell->exitCode() == 0
        ? EXIT_SUCCESS : EXIT_FAILURE);
}

void Shell::handleStdin()
{
    m_shell->write(m_stdin->readLine());
}

