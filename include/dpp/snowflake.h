/************************************************************************************
 *
 * D++, A Lightweight C++ library for Discord
 *
 * Copyright 2021 Craig Edwards and D++ contributors 
 * (https://github.com/brainboxdotcc/DPP/graphs/contributors)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ************************************************************************************/
#pragma once
#include <dpp/export.h>
#include <dpp/json_fwd.h>
#include <stdint.h>
#include <functional>

/**
 * @brief The main namespace for D++ functions. classes and types
 */
namespace dpp {

/** @brief A container for a 64 bit unsigned value representing many things on discord.
 * This value is known in distributed computing as a snowflake_t value.
 * 
 * snowflake_ts are:
 * 
 * - Performant (very fast to generate at source and to compare in code)
 * - Uncoordinated (allowing high availability across clusters, data centres etc)
 * - Time ordered (newer snowflake_ts have higher IDs)
 * - Directly Sortable (due to time ordering)
 * - Compact (64 bit numbers, not 128 bit, or string)
 * 
 * An identical format of snowflake_t is used by Twitter, Instagram and several other platforms.
 * 
 * @see https://en.wikipedia.org/wiki/snowflake_t_ID
 * @see https://github.com/twitter-archive/snowflake_t/tree/b3f6a3c6ca8e1b6847baa6ff42bf72201e2c2231
 */
template <typename T = void>
class snowflake_t;

template <>
class DPP_EXPORT snowflake_t<void> {
	friend struct std::hash<dpp::snowflake_t<void>>;
protected:
	/**
	 * @brief The snowflake_t value
	 */
	uint64_t value;

public:
	/**
	 * @brief Construct a snowflake_t object
	 * @param value A snowflake_t value
	 */
	snowflake_t(const uint64_t& value);

	/**
	 * @brief Construct a snowflake_t object
	 * @param string_value A snowflake_t value
	 */
	snowflake_t(const std::string& string_value);

	/**
 	 * @brief Construct a snowflake_t object
 	 */
	snowflake_t();

	/**
	 * @brief Destroy the snowflake_t object
	 */
	~snowflake_t() = default;

	/**
	 * @brief For acting like an integer
	 * @return The snowflake_t value
	 */
	operator uint64_t() const;

	/**
	 * @brief Returns true if the snowflake_t holds an empty value (is 0)
	 * 
	 * @return true if empty (zero)
	 */
	inline bool empty() const
	{
		return value == 0;
	}

	/**
	 * @brief Operator less than, used for maps/unordered maps
	 * when the snowflake_t is a key value.
	 * 
	 * @param lhs fist snowflake_t to compare
	 * @param rhs second snowflake_t to compare
	 * @return true if lhs is less than rhs
	 */
	friend inline bool operator< (const snowflake_t& lhs, const snowflake_t& rhs)
	{
		return lhs.value < rhs.value;
	}

	/**
	 * @brief Assign from std::string
	 * 
	 * @param snowflake_t_val string to assign from.
	 */
	snowflake_t& operator=(const std::string &snowflake_t_val);

	/**
	 * @brief Assign from std::string
	 * 
	 * @param snowflake_t_val value to assign from.
	 */
	snowflake_t& operator=(const uint64_t &snowflake_t_val);

	/**
	 * @brief Check if one snowflake_t value is equal to another
	 * 
	 * @param other other snowflake_t to compare
	 * @return True if the snowflake_t objects match
	 */
	bool operator==(const snowflake_t& other) const;

	/**
	 * @brief Check if one snowflake_t value is equal to a uint64_t
	 * 
	 * @param other other snowflake_t to compare
	 * @return True if the snowflake_t objects match
	 */
	bool operator==(const uint64_t& other) const;

	/**
	 * @brief For building json
	 * @return The snowflake_t value as a string
	 */
	operator nlohmann::json() const;

	/**
	 * @brief Get the creation time of this snowflake_t according to Discord.
	 * 
	 * @return double creation time inferred from the snowflake_t ID.
	 * The minimum possible value is the first second of 2015.
	 */
	double get_creation_time() const;

	/**
	 * @brief Get the worker id that produced this snowflake_t value
	 * 
	 * @return uint8_t worker id
	 */
	uint8_t get_worker_id() const;

	/**
	 * @brief Get the process id that produced this snowflake_t value
	 * 
	 * @return uint8_t process id
	 */
	uint8_t get_process_id() const;

	/**
	 * @brief Get the increment, which is incremented for every snowflake_t
	 * created over the one millisecond resolution in the timestamp.
	 * 
	 * @return uint64_t millisecond increment
	 */
	uint16_t get_increment() const;
};

template <typename T>
class snowflake_t final : public snowflake_t<void> {
	friend struct std::hash<dpp::snowflake_t<T>>;
public:
	/**
	 * @brief Construct a snowflake_t object
	 * @param value A snowflake_t value
	 */
	template <typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
	snowflake_t(U value) : snowflake_t<void>(value) {

	}

	/**
	 * @brief Construct a snowflake_t object
	 * @param string_value A snowflake_t value
	 */
	snowflake_t(const std::string& string_value) : snowflake_t<void>(string_value) {
	}
	
	/**
	 * @brief Construct a snowflake_t object
	 * @param value A snowflake_t
	 */
	snowflake_t(const snowflake_t &value) : snowflake_t<void>(value.value) {
	}
	
	/**
	 * @brief Any to specific, and guild snowflake to @everyone role snowflake conversion
	 * @param value A snowflake_t
	 */
	template <typename U, typename = std::enable_if_t<std::is_same_v<U, void> || std::is_same_v<T, role> && std::is_same_v<U, guild>>>
	snowflake_t(const snowflake_t<U> &value) : snowflake_t<void>(static_cast<uint64_t>(value)) {
	}

	/**
 	 * @brief Construct a snowflake_t object
 	 */
	snowflake_t() = default;

	/**
	 * @brief Destroy the snowflake_t object
	 */
	~snowflake_t() = default;

	using snowflake_t<void>::operator nlohmann::json;

	using snowflake_t<void>::operator size_t;
};

snowflake_t(uint64_t) -> snowflake_t<void>;

using snowflake = snowflake_t<void>;

};

template<>
struct std::hash<dpp::snowflake_t<void>>
{
	/**
	 * @brief Hashing function for dpp::slowflake
	 * Used by std::unordered_map. This just calls std::hash<uint64_t>.
	 * 
	 * @param s snowflake_t value to hash
	 * @return std::size_t hash value
	 */
	std::size_t operator()(dpp::snowflake_t<void> const& s) const noexcept {
		return std::hash<uint64_t>{}(s.value);
	}
};

template <typename T>
struct std::hash<dpp::snowflake_t<T>> {
	/**
	 * @brief Hashing function for dpp::slowflake
	 * Used by std::unordered_map. This just calls std::hash<uint64_t>.
	 * 
	 * @param s snowflake_t value to hash
	 * @return std::size_t hash value
	 */
	std::size_t operator()(dpp::snowflake_t<T> const& s) const noexcept {
		return std::hash<uint64_t>{}(s.value);
	}
};
