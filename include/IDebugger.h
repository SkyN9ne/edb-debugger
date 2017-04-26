/*
Copyright (C) 2006 - 2015 Evan Teran
                          evan.teran@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef IDEBUGGER_20061101_H_
#define IDEBUGGER_20061101_H_

#include "Types.h"

#include <QByteArray>
#include <QHash>
#include <QMap>
#include <QString>
#include <QtPlugin>

#include <memory>

class IState;
class State;
class Status;
class IBreakpoint;
class IDebugEvent;
class IProcess;

class IDebugger {
public:
	typedef QHash<edb::address_t, std::shared_ptr<IBreakpoint>> BreakpointList;

public:
	virtual ~IDebugger() = default;

public:
	// system properties
    virtual edb::address_t           page_size() const = 0;
    virtual std::size_t              pointer_size() const = 0;
    virtual quint64                  cpu_type() const = 0;
    virtual bool                     has_extension(quint64 ext) const = 0;
    virtual QMap<qlonglong, QString> exceptions() const = 0;
    virtual QString                  exceptionName(qlonglong value) = 0;
    virtual qlonglong                exceptionValue(const QString &name) = 0;

public:
	// important register names
	virtual QString stack_pointer() const = 0;
	virtual QString frame_pointer() const = 0;
	virtual QString instruction_pointer() const = 0;
	virtual QString flag_register() const = 0;

public:
	// data output
	virtual QString format_pointer(edb::address_t address) const = 0;

public:
	// general process data
	virtual edb::pid_t parent_pid(edb::pid_t pid) const = 0;
	virtual QMap<edb::pid_t, std::shared_ptr<IProcess>> enumerate_processes() const = 0;

public:
	// basic process management
	virtual Status attach(edb::pid_t pid) = 0;
	virtual Status open(const QString &path, const QString &cwd, const QList<QByteArray> &args) = 0;
	virtual Status open(const QString &path, const QString &cwd, const QList<QByteArray> &args, const QString &tty) = 0;
	virtual std::shared_ptr<IDebugEvent> wait_debug_event(int msecs) = 0;
	virtual void detach() = 0;
	virtual void kill() = 0;
	virtual void end_debug_session() = 0;
	virtual void get_state(State *state) = 0;
	virtual void set_state(const State &state) = 0;

public:
	// basic breakpoint managment
	virtual BreakpointList               backup_breakpoints() const = 0;
	virtual std::shared_ptr<IBreakpoint> add_breakpoint(edb::address_t address) = 0;
	virtual std::shared_ptr<IBreakpoint> find_breakpoint(edb::address_t address) = 0;
	virtual void                         clear_breakpoints() = 0;
	virtual void                         remove_breakpoint(edb::address_t address) = 0;

public:
	virtual IState *create_state() const = 0;

public:
	// NULL if not attached
	virtual IProcess *process() const = 0;
};

Q_DECLARE_INTERFACE(IDebugger, "edb.IDebugger/1.0")

#endif
