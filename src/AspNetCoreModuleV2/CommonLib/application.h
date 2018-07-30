// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#pragma once

#include "iapplication.h"
#include "exceptions.h"
#include "utility.h"
#include "ntassert.h"


class APPLICATION : public IAPPLICATION
{

public:
    // Non-copyable
    APPLICATION(const APPLICATION&) = delete;
    const APPLICATION& operator=(const APPLICATION&) = delete;

    APPLICATION_STATUS
    QueryStatus() override
    {
        return m_status;
    }

    APPLICATION()
        : m_cRefs(1)
    {
        InitializeSRWLock(&m_stateLock);
    }

    
    VOID
    Stop(bool fServerInitiated) override
    {
        UNREFERENCED_PARAMETER(fServerInitiated);

        m_fStopCalled = true;
    }
    

    VOID
    ReferenceApplication() override
    {
        DBG_ASSERT(m_cRefs > 0);

        InterlockedIncrement(&m_cRefs);
    }

    VOID
    DereferenceApplication() override
    {
        DBG_ASSERT(m_cRefs > 0);

        if (InterlockedDecrement(&m_cRefs) == 0)
        {
            delete this;
        }
    }

protected:
    volatile APPLICATION_STATUS     m_status = APPLICATION_STATUS::UNKNOWN;
    SRWLOCK m_stateLock;
    bool m_fStopCalled;

private:

    mutable LONG                    m_cRefs;
};
