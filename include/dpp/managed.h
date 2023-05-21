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
#include <dpp/snowflake.h>
#include <string>

namespace dpp {
	namespace detail {
		/**
		 * @brief Internal class that is the base of a managed object
		 * For caching and polymorphism purposes, do not use
		 */
		class DPP_EXPORT managed_base {
		public:
			/**
			 * @brief Destroy the managed object
			 */
			virtual ~managed_base() = default;
		};
	}

	/** @brief The managed class is the base class for various types that can
	 * be stored in a cache that are identified by a dpp::snowflake id.
	 */
	template <typename T>
	class managed : public detail::managed_base {
	public:
		using object_type = T;

		/**
		 * @brief Unique ID of object set by Discord.
		 * This value contains a timestamp, worker ID, internal server ID, and an incrementing value.
		 * Only the timestamp is relevant to us as useful metadata.
		 */
		snowflake_t<T> id;

		/**
		 * @brief Constructor, initialises ID
		 * @param nid ID to set
		 */
		explicit managed(const snowflake_t<T> nid = snowflake_t<T>{0}) : id{nid} {
		}

		/**
		 * @brief Destroy the managed object
		 */
		virtual ~managed() override = default;

		/**
		 * @brief Get the creation time of this object according to Discord.
		 * 
		 * @return double creation time inferred from the snowflake ID.
		 * The minimum possible value is the first second of 2015.
		 */
		double get_creation_time() const {
			return this->id.get_creation_time();
		}

		/**
		 * @brief Comparison operator for comparing two managed objects by id
		 * 
		 * @param other Other object to compare against
		 * @return true objects are the same id
		 * @return false objects are not the same id
		 */
		bool operator==(const managed& other) const noexcept {
			return id == other.id;
		}

		/**
		 * @brief Comparison operator for comparing two managed objects by id
		 * 
		 * @param other Other object to compare against
		 * @return true objects are not the same id
		 * @return false objects are the same id
		 */
		bool operator!=(const managed& other) const noexcept {
			return !(id == other.id);
		}
	};

};
