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
#ifndef _STREAM_H_
#define _STREAM_H_

#include <cstddef>
#include <string>
#include <type_traits>
#include <vector>

namespace cat {
	/*
	 * The _stream class provides a simple wrapper around a dynamic byte buffer.
	 * It allows reading the current buffer, querying its size, and clearing its contents
	 * while retaining the allocated memory capacity for efficient reuse.
	 */
	class _stream {
	protected:
		using byte_type = std::byte;
		using buffer_type = std::vector<byte_type>;
	public:
		constexpr _stream() noexcept = default;

		/*
		 * Constructs a stream from a raw byte buffer range.
		 *
		 * @param _Begin Pointer to the beginning of the byte buffer
		 * @param _End   Pointer to one past the last byte of the buffer
		 */
		_stream(const byte_type* _Begin, const byte_type* _End)
			: buf(_Begin, _End) {
		}

		/*
		 * Constructs a stream with an existing byte buffer.
		 *
		 * @param _Buffer A constant reference to a vector of bytes used to initialize the stream
		 */
		_stream(const buffer_type& _Buffer)
			: buf(_Buffer) {
		}

		/*
		 * Constructs a stream by transferring ownership of an existing byte buffer.
		 *
		 * @param _Buffer An rvalue reference to a byte buffer whose contents are moved into the stream
		 */
		_stream(buffer_type&& _Buffer) noexcept
			: buf(std::move(_Buffer)) {
		}

		/*
		 * Returns a constant reference to the internal byte buffer.
		 * Provides read-only access to the underlying storage.
		 *
		 * @return A constant reference to the internal byte buffer
		 */
		[[nodiscard]] constexpr const buffer_type& buffer() const noexcept {
			return buf;
		}

		/*
		 * Returns the number of bytes currently stored in the buffer.
		 *
		 * @return The size of the internal byte buffer
		 */
		[[nodiscard]] constexpr std::size_t size() const noexcept {
			return buf.size();
		}

		/*
		 * Clears all data in the buffer but retains the allocated capacity.
		 * This enables efficient reuse of the buffer without triggering reallocation.
		 */
		constexpr void flush() noexcept {
			buf.clear();
		}

		/*
		 * Pre-allocates internal buffer capacity to at least `_Size` bytes.
		 *
		 * This allows you to reserve enough space in advance if you roughly know
		 * how much data will be written. By doing so, you can avoid multiple
		 * reallocations during successive writes, improving performance.
		 *
		 * @param _Size Minimum number of bytes to reserve in the internal buffer
		 */
		constexpr void reserve(std::size_t _Size) noexcept {
			buf.reserve(_Size);
		}
	protected:
		// Internal storage for raw byte data
		buffer_type buf;
	};

	/*
	 * Input stream class for reading from a _stream buffer.
	 * Supports copy, move, and raw pointer initialization, and provides
	 * multiple read methods for primitive types and strings.
	 */
	class istream : public _stream {
	public:
		/*
		 * Constructs an input stream from a raw byte buffer.
		 *
		 * @param _Buffer Pointer to the beginning of the byte buffer
		 * @param _Size   Number of bytes to copy from the buffer
		 */
		istream(const byte_type* _Buffer, std::size_t _Size)
			: _stream(_Buffer, _Buffer + _Size), pos(0) {
		}

		/*
		 * Constructs an input stream by copying from an existing vector of bytes.
		 *
		 * @param _Buffer A constant reference to a vector of bytes to copy
		 */
		istream(const buffer_type& _Buffer)
			: _stream(_Buffer), pos(0) {
		}

		/*
		 * Constructs an input stream by moving an existing vector of bytes.
		 *
		 * @param _Buffer An rvalue reference to a vector of bytes
		 */
		istream(buffer_type&& _Buffer) noexcept
			: _stream(std::move(_Buffer)), pos(0) {
		}

		/*
		 * Reads a trivially copyable type from the stream.
		 * Performs strict bounds checking and advances the read position
		 * on successful extraction.
		 *
		 * @tparam _Ty  A trivially copyable type to be read
		 * @param  _Value Reference to the output variable
		 *
		 * @return true  If the required number of bytes is available
		 * @return false If the buffer does not contain enough data
		 */
		template<class _Ty>
		bool read(_Ty& _Value) noexcept {
			static_assert(std::is_trivially_copyable_v<_Ty>, "requires trivially copyable type");
			constexpr std::size_t SIZE = sizeof(_Ty);
			if (pos + SIZE > buf.size()) {
				return false;
			}
			std::memcpy(&_Value, buf.data() + pos, SIZE);
			pos += SIZE;
			return true;
		}

		/*
		 * Reads a length-prefixed string from the stream buffer.
		 *
		 * The function first reads the size prefix (size_t), checks if
		 * there are enough bytes remaining in the buffer, and then
		 * extracts the string into _Value.
		 *
		 * @param _Value Reference to the std::string where the result will be stored
		 * @return true  If the string was successfully read
		 * @return false If there is not enough data left in the buffer
		 */
		bool read_str(std::string& _Value) noexcept {
			std::size_t size;
			if (!read(size) || pos + size > buf.size()) {
				return false;
			}
			_Value.resize(size);
			std::memcpy(_Value.data(), buf.data() + pos, size);
			pos += size;
			return true;
		}
	private:
		// Current read position within the buffer
		std::size_t pos;
	};

	/*
	 * The ostream class provides a write-oriented interface for managing
	 * a dynamic byte buffer. It extends the base _stream functionality by
	 * offering mechanisms to construct and manipulate output data.
	 */
	class ostream : public _stream {
	public:
		constexpr ostream() = default;

		/*
		 * Constructs an output stream by moving an existing byte buffer.
		 *
		 * @param _Buffer An rvalue reference to a vector of bytes to take ownership of
		 */
		ostream(buffer_type&& _Buffer) noexcept
			: _stream(std::move(_Buffer)) {
		}

		/*
		 * Writes a trivially copyable type into the stream.
		 * The type is copied as raw bytes into the internal buffer.
		 *
		 * @tparam _Ty    A trivially copyable type
		 * @param  _Value A constant reference to the value to be written
		 */
		template<class _Ty>
		void write(const _Ty& _Value) {
			static_assert(std::is_trivially_copyable_v<_Ty>, "requires trivially copyable type");
			constexpr std::size_t SIZE = sizeof(_Ty);
			buf.resize(buf.size() + SIZE);
			std::memcpy(buf.data() + buf.size() - SIZE, &_Value, SIZE);
		}

		/*
		 * Writes a length-prefixed string into the stream buffer.
		 *
		 * The string is stored as a size prefix (size_t) followed by the raw
		 * string data (not null-terminated). This allows the corresponding
		 * read_str function to know exactly how many bytes to read.
		 *
		 * @param _Value The std::string to write into the buffer
		 */
		void write_str(const std::string& _Value) {
			write(_Value.size());
			const std::size_t size = buf.size();
			buf.resize(buf.size() + _Value.size());
			std::memcpy(buf.data() + size, _Value.data(), _Value.length());
		}
	private:
	};
}

#endif // ^^^ !_STREAM_H_