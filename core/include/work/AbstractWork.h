#pragma once

// Copyright 2015 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include <deque>
#include <map>
#include <memory>
#include <string>
#include <util/noncopyable.h>

namespace vixal {

class Application;

class Work;

typedef std::shared_ptr<Work> work_ptr;

/**
 * AbstractWork is a class of things-that-hold Work, and are notified by work when it completes. This is an abstract
 * base that's implemented by both WorkManager and Work itself.
 *
 * It also has a utility method addWork<W>(...) for subclasses of Work; these are constructed with appropriate
 * application and parent links and automatically added to the child list.
 */
class AbstractWork : public std::enable_shared_from_this<AbstractWork>, private nonmovableorcopyable {
protected:
    Application &mApp;
    std::map<std::string, work_ptr> mChildren;

public:
    explicit AbstractWork(Application &app);

    virtual void notify(std::string const &childChanged) = 0;

    void addChild(work_ptr child);

    void clearChildren();

    void advanceChildren();

    bool anyChildRaiseFailure() const;

    bool anyChildFatalFailure() const;

    bool allChildrenSuccessful() const;

    bool allChildrenDone() const;

    Application &app() const;

    template<typename T, typename... Args>
    std::shared_ptr<T>
    addWork(Args &&... args) {
        auto w = std::make_shared<T>(mApp, *this, std::forward<Args>(args)...);
        addChild(w);
        return w;
    }

};

}
