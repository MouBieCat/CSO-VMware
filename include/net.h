/***
* MIT License
*
* Copyright (c) 2025 MouBieCat
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
***/

#pragma once
#ifndef _NET_H_
#define _NET_H_

namespace cat {
    /*
     * Reports whether the ENet subsystem has been successfully initialized.
     *
     * @return true  If the ENet system is initialized and ready.
     * @return false If ENet has not been set up or has already been shut down.
     */
    [[nodiscard]] bool is_setup() noexcept;

    /*
     * Returns whether a client connection attempt is currently active.
     *
     * @return true  If a connection is active or being established.
     * @return false If no client connection exists.
     */
	[[nodiscard]] bool is_connecting() noexcept;
}

#endif // ^^^ !_NET_H_