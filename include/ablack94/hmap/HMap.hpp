#ifndef HMAP_HPP
#define HMAP_HPP

#include <atomic>
#include <cstdint>
#include <map>
#include <string>
#include <memory>
#include <iostream>

typedef uint64_t HMapSettingId;

namespace ablack94 {
namespace hmap {

namespace detail {
	// Singleton used to manage the current type ID counter
	class _HMapGlobals {
	public:
		static _HMapGlobals& getInstance() {
			static _HMapGlobals instance;
			return instance;
		}

		_HMapGlobals() = default;
		_HMapGlobals(_HMapGlobals const&) = delete;
		void operator=(_HMapGlobals const&) = delete;

		std::atomic<HMapSettingId> current_id;
	};
} // detail

class BaseHMapSetting {
public:
	BaseHMapSetting() = default;

	virtual HMapSettingId getType() = 0;
};

template <typename T>
class HMapSetting : public BaseHMapSetting {
public:
	HMapSetting() {
		this->value = nullptr;
	}

	HMapSetting(T value) {
		this->value = value;
	}

	static HMapSettingId Type() {
		// Gets a unique type ID for this class (each different template 'T' has a different value)
		static HMapSettingId id = detail::_HMapGlobals::getInstance().current_id++;
		return id;
	}

	HMapSettingId getType() override {
		return this->Type();
	}

	T* getValue() { return &value; }

protected:
	T value;
};

class HMap {
public:
	HMap() = default;
	// TODO: Not sure about copy/move constructors

	template <typename T, typename ...Args>
	void set(std::string name, Args&&... args) {
		// TODO: Should be able to use move semantics to avoid copying 'v'
		T v(args...);
		settings[name] = std::make_unique<HMapSetting<T>>(v);
	}

	BaseHMapSetting* get(std::string name) {
		return settings[name].get();
	}

	BaseHMapSetting* operator[](std::string name) {
		return get(name);
	}

	template <typename T>
	T* get(std::string name) {
		if (settings.count(name) > 0) {
			BaseHMapSetting *ptr = settings[name].get();
			if (ptr->getType() == HMapSetting<T>::Type()) {
				return static_cast<HMapSetting<T>*>(ptr)->getValue();
			}
			else {
				throw std::exception("Bad type");
			}
		}
		return nullptr;
	}

	HMapSettingId getId(std::string name) {
		return settings[name]->getType();
	}

protected:
	std::map<std::string, std::unique_ptr<BaseHMapSetting>> settings;
};

} // hmap
} // ablack94

#endif