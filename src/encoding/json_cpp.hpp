#pragma once
#include <charconv>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace json {

struct BoolValue;
struct StringValue;
struct IntValue;
struct FloatValue;
struct ListValue;
struct ObjectValue;

struct Value {
	virtual ~Value() = default;
	virtual void print(std::ostream &os) const = 0;
	virtual BoolValue* tryBool() = 0;
	virtual const BoolValue* tryBool() const = 0;
	virtual StringValue* tryString() = 0;
	virtual const StringValue* tryString() const = 0;
	virtual IntValue* tryInt() = 0;
	virtual const IntValue* tryInt() const = 0;
	virtual FloatValue* tryFloat() = 0;
	virtual const FloatValue* tryFloat() const = 0;
	virtual ListValue* tryList() = 0;
	virtual const ListValue* tryList() const = 0;
	virtual ObjectValue* tryObject() = 0;
	virtual const ObjectValue* tryObject() const = 0;
	virtual bool isNull() const = 0;
};

using ValuePtr = std::unique_ptr<Value>;
using Object = std::map<std::string, ValuePtr>;

struct BoolValue : public Value {
	BoolValue(bool boolean) : value(boolean) {}

	operator bool& () {
		return value;
	}

	operator const bool& () const {
		return value;
	}

	void print(std::ostream &os) const override {
		os << (value ? "true" : "false");
	}

	BoolValue* tryBool() override {
		return this;
	}

	const BoolValue* tryBool() const override {
		return this;
	}

	StringValue* tryString() override {
		return nullptr;
	}

	const StringValue* tryString() const override {
		return nullptr;
	}

	IntValue* tryInt() override {
		return nullptr;
	}

	const IntValue* tryInt() const override {
		return nullptr;
	}

	FloatValue* tryFloat() override {
		return nullptr;
	}

	const FloatValue* tryFloat() const override {
		return nullptr;
	}

	ListValue* tryList() override {
		return nullptr;
	}

	const ListValue* tryList() const override {
		return nullptr;
	}

	virtual ObjectValue* tryObject() override {
		return nullptr;
	}

	virtual const ObjectValue* tryObject() const override {
		return nullptr;
	}

	virtual bool isNull() const override {
		return false;
	}

	bool value;
};

struct StringValue : public Value {
	StringValue(const std::string& str) : value(str) {}
	StringValue(const std::string_view str) : value(str) {}

	operator std::string&() {
		return value;
	}

	operator const std::string&() const {
		return value;
	}

	operator std::string_view() const {
		return value;
	}

	void print(std::ostream &os) const override {
		os << value;
	}

	BoolValue* tryBool() override {
		return nullptr;
	}

	const BoolValue* tryBool() const override {
		return nullptr;
	}

	StringValue* tryString() override {
		return this;
	}

	const StringValue* tryString() const override {
		return this;
	}

	IntValue* tryInt() override {
		return nullptr;
	}

	const IntValue* tryInt() const override {
		return nullptr;
	}

	FloatValue* tryFloat() override {
		return nullptr;
	}

	const FloatValue* tryFloat() const override {
		return nullptr;
	}

	ListValue* tryList() override {
		return nullptr;
	}

	const ListValue* tryList() const override {
		return nullptr;
	}

	virtual ObjectValue* tryObject() override {
		return nullptr;
	}

	virtual const ObjectValue* tryObject() const override {
		return nullptr;
	}

	virtual bool isNull() const override {
		return false;
	}

	std::string value;
};

struct IntValue : public Value {
	IntValue(int64_t integer) : value(integer) {}

	operator int64_t& () {
		return value;
	}

	operator const int64_t& () const {
		return value;
	}

	void print(std::ostream &os) const override {
		os << value;
	}

	BoolValue* tryBool() override {
		return nullptr;
	}

	const BoolValue* tryBool() const override {
		return nullptr;
	}

	StringValue* tryString() override {
		return nullptr;
	}

	const StringValue* tryString() const override {
		return nullptr;
	}

	IntValue* tryInt() override {
		return this;
	}

	const IntValue* tryInt() const override {
		return this;
	}

	FloatValue* tryFloat() override {
		return nullptr;
	}

	const FloatValue* tryFloat() const override {
		return nullptr;
	}

	ListValue* tryList() override {
		return nullptr;
	}

	const ListValue* tryList() const override {
		return nullptr;
	}

	virtual ObjectValue* tryObject() override {
		return nullptr;
	}

	virtual const ObjectValue* tryObject() const override {
		return nullptr;
	}

	virtual bool isNull() const override {
		return false;
	}

	int64_t value;
};

struct FloatValue : public Value {
	FloatValue(double real) : value(real) {}

	operator double& () {
		return value;
	}

	operator const double& () const {
		return value;
	}

	void print(std::ostream &os) const override {
		os << value;
	}

	BoolValue* tryBool() override {
		return nullptr;
	}

	const BoolValue* tryBool() const override {
		return nullptr;
	}

	StringValue* tryString() override {
		return nullptr;
	}

	const StringValue* tryString() const override {
		return nullptr;
	}

	IntValue* tryInt() override {
		return nullptr;
	}

	const IntValue* tryInt() const override {
		return nullptr;
	}

	FloatValue* tryFloat() override {
		return this;
	}

	const FloatValue* tryFloat() const override {
		return this;
	}

	ListValue* tryList() override {
		return nullptr;
	}

	const ListValue* tryList() const override {
		return nullptr;
	}

	virtual ObjectValue* tryObject() override {
		return nullptr;
	}

	virtual const ObjectValue* tryObject() const override {
		return nullptr;
	}

	virtual bool isNull() const override {
		return false;
	}

	double value;
};

struct ListValue : public Value {

	operator std::vector<ValuePtr>& () {
		return value;
	}

	operator const std::vector<ValuePtr>& () const {
		return value;
	}

	void print(std::ostream &os) const override {
		os << '[';
		for(size_t i = 0; i < value.size(); ++i) {
			value[i]->print(os);
			if(i != value.size() - 1) {
				os << ", ";
			}
		}
		os << ']';
	}

	BoolValue* tryBool() override {
		return nullptr;
	}

	const BoolValue* tryBool() const override {
		return nullptr;
	}

	StringValue* tryString() override {
		return nullptr;
	}

	const StringValue* tryString() const override {
		return nullptr;
	}

	IntValue* tryInt() override {
		return nullptr;
	}

	const IntValue* tryInt() const override {
		return nullptr;
	}

	FloatValue* tryFloat() override {
		return nullptr;
	}

	const FloatValue* tryFloat() const override {
		return nullptr;
	}

	ListValue* tryList() override {
		return this;
	}

	const ListValue* tryList() const override {
		return this;
	}

	virtual ObjectValue* tryObject() override {
		return nullptr;
	}

	virtual const ObjectValue* tryObject() const override {
		return nullptr;
	}

	virtual bool isNull() const override {
		return false;
	}

	std::vector<ValuePtr> value;
};

struct ObjectValue : public Value {
	void print(std::ostream &os) const override {
		os << '{';
		if(!value.empty()) {
			auto last = --value.cend();
			for(auto  it = value.cbegin(); it != value.cend(); ++it) {
				os << it->first << ": ";
				it->second->print(os);
				if(it != last) {
					os << ", ";
				}
			}
		}
		os << '}';
	}

	BoolValue* tryBool() override {
		return nullptr;
	}

	const BoolValue* tryBool() const override {
		return nullptr;
	}

	StringValue* tryString() override {
		return nullptr;
	}

	const StringValue* tryString() const override {
		return nullptr;
	}

	IntValue* tryInt() override {
		return nullptr;
	}

	const IntValue* tryInt() const override {
		return nullptr;
	}

	FloatValue* tryFloat() override {
		return nullptr;
	}

	const FloatValue* tryFloat() const override {
		return nullptr;
	}

	ListValue* tryList() override {
		return nullptr;
	}

	const ListValue* tryList() const override {
		return nullptr;
	}

	virtual ObjectValue* tryObject() override {
		return this;
	}

	virtual const ObjectValue* tryObject() const override {
		return this;
	}

	Value* lookup(const std::string &key) {
		auto it = value.find(key);
		if(it == value.end()) {
			return nullptr;
		}
		return it->second.get();
	}

	const Value* lookup(const std::string &key) const {
		auto it = value.find(key);
		if(it == value.end()) {
			return nullptr;
		}
		return it->second.get();
	}

	auto lookupAsBool(const std::string& key) const -> const json::BoolValue* {
		if(auto boolean = this->lookup(key); boolean != nullptr) {
			return boolean->tryBool();
		}
		return nullptr;
	}

	auto lookupAsString(const std::string& key) const -> const json::StringValue* {
		if(auto string = this->lookup(key); string != nullptr) {
			return string->tryString();
		}
		return nullptr;
	}

	auto lookupAsInt(const std::string& key) const -> const json::IntValue* {
		if(auto integer = this->lookup(key); integer != nullptr) {
			return integer->tryInt();
		}
		return nullptr;
	}

	auto lookupAsFloat(const std::string& key) const -> const json::FloatValue* {
		if(auto fp = this->lookup(key); fp != nullptr) {
			return fp->tryFloat();
		}
		return nullptr;
	}

	auto lookupAsList(const std::string& key) const -> const json::ListValue* {
		if(auto list = this->lookup(key); list != nullptr) {
			return list->tryList();
		}
		return nullptr;
	}

	auto lookupAsObject(const std::string& key) const -> const json::ObjectValue* {
		if(auto object = this->lookup(key); object != nullptr) {
			return object->tryObject();
		}
		return nullptr;
	}

	virtual bool isNull() const override {
		return false;
	}

	Object value;
};

struct NullValue : public Value {

	void print(std::ostream &os) const override {
		os << "null";
	}

	BoolValue* tryBool() override {
		return nullptr;
	}

	const BoolValue* tryBool() const override {
		return nullptr;
	}

	StringValue* tryString() override {
		return nullptr;
	}

	const StringValue* tryString() const override {
		return nullptr;
	}

	IntValue* tryInt() override {
		return nullptr;
	}

	const IntValue* tryInt() const override {
		return nullptr;
	}

	FloatValue* tryFloat() override {
		return nullptr;
	}

	const FloatValue* tryFloat() const override {
		return nullptr;
	}

	ListValue* tryList() override {
		return nullptr;
	}

	const ListValue* tryList() const override {
		return nullptr;
	}

	virtual ObjectValue* tryObject() override {
		return nullptr;
	}

	virtual const ObjectValue* tryObject() const override {
		return nullptr;
	}

	virtual bool isNull() const override {
		return true;
	}

	std::vector<ValuePtr> value;
};

class Encoder {
public:
	void startEncoding() {
		result = '{';
	}

	template<typename ValueType>
	void insert(std::string_view key, const ValueType &value) {
		insertImpl(key, value);
	}

	template<typename ValueType>
	void insert(std::string_view key, const std::initializer_list<ValueType> &list) {
		insertImpl(key, list);
	}

	void stopEncoding() {
		if(!result.empty() && result.back() == ',') {
			result.pop_back();
		}
		result.push_back('}');
	}

	const std::string &str() const {
		return result;
	}

private:
	template<typename ValueType>
	void insertImpl(std::string_view key, const ValueType &value) {
		result += '"';
		result += key;
		result += '"';
		result += ':';
		put(value);
		result += ',';
	}

	void put(const bool value) {
		result += value ? "true" : "false";
	}

	void put(const std::string &value) {
		result += '"';
		result += value;
		result += '"';
	};

	void put(const char* value) {
		result += '"';
		result += value;
		result += '"';
	}

	void put(const int &value) {
		result += std::to_string(value);
	}

	void put(const double &value) {
		result += std::to_string(value);
	}

	template<typename ListType>
	void put(const ListType &list) {
		result += '[';
		for(const auto &elem : list) {
			put(elem);
			result += ',';
		}
		result.pop_back();
		result += ']';
	}

	std::string result;
};

class Decoder {
public:
	ValuePtr decode(std::string_view view) {
		sv = view;
		auto object = parseObject();
		if(object == nullptr) {
			auto list = parseList();
			return ValuePtr(list.release());
		}
		return ValuePtr(object.release());
	}
private:
	ValuePtr parseObject() {
		auto oldObject = activeObject;
		auto object = std::make_unique<ObjectValue>();
		activeObject = &object->value;
		ignoreWhitespace();
		if(!has('{')) {
			activeObject = oldObject;
			return nullptr;
		}
		
		next();

		ignoreWhitespace();
		while(!has('}')) {		
			ignoreWhitespace();
			if(parseKeyValue()) {
				ignoreWhitespace();
				continue;
			}
			activeObject = oldObject;
			return nullptr;
		}
		next();

		activeObject = oldObject;
		return object;
	}

	bool parseKeyValue() {
		std::string_view key = parseKey();
		if(key.empty()) {
			return false;
		}

		ignoreWhitespace();

		ValuePtr ptr = parseValue();
		if(ptr == nullptr) {
			return false;
		}

		activeObject->insert({std::string(key), std::move(ptr)});

		return true;
	}

	std::string_view parseKey() {
		std::string_view view = parseString();
		if(view.empty()) {
			return {};
		}

		ignoreWhitespace();

		if(!has(':')) {
			return {};
		}

		next();
		ignoreWhitespace();

		return view;
	}

	ValuePtr parseValue() {
		void* successLabel = nullptr;
		ValuePtr ptr;

		std::string_view view = parseString();
		if(!view.empty()) {
			successLabel = &&SUCCESS_STRING;
			goto SUCCESS_PARSE_COMMA;
SUCCESS_STRING:
			return std::make_unique<StringValue>(view);
		}

		ptr = parseFloat();
		if(ptr != nullptr) {
			successLabel = &&SUCCESS_FLOAT;
			goto SUCCESS_PARSE_COMMA;
SUCCESS_FLOAT:
			return ptr;
		}

		ptr = parseInt();
		if(ptr != nullptr) {
			successLabel = &&SUCCESS_INTEGER;
			goto SUCCESS_PARSE_COMMA;
SUCCESS_INTEGER:
			return ptr;
		}

		ptr = parseList();
		if(ptr != nullptr) {
			successLabel = &&SUCCESS_LIST;
			goto SUCCESS_PARSE_COMMA;
SUCCESS_LIST:
			return ptr;
		}

		ptr = parseObject();
		if(ptr != nullptr) {
			successLabel = &&SUCCESS_OBJECT;
			goto SUCCESS_PARSE_COMMA;
SUCCESS_OBJECT:
			return ptr;
		}

		ptr = parseBool();
		if(ptr != nullptr) {
			successLabel = &&SUCCESS_BOOL;
			goto SUCCESS_PARSE_COMMA;
SUCCESS_BOOL:
			return ptr;
		}

		ptr = parseNull();
		if(ptr != nullptr) {
			successLabel = &&SUCCESS_NULL;
			goto SUCCESS_PARSE_COMMA;
SUCCESS_NULL:
			return ptr;
		}

SUCCESS_PARSE_COMMA:
		if(successLabel) {
			ignoreWhitespace();
			if(has(',')) {
				next();
			}
			goto *successLabel;
		}

		return nullptr;
	}

	ValuePtr parseList() {
		if(!has('[')) {
			return nullptr;
		}
		next();

		auto list = std::make_unique<ListValue>();

		ignoreWhitespace();
		while(!has(']')) {
			ignoreWhitespace();
			auto value = parseValue();
			if(value == nullptr) {
				return nullptr;
			}

			list->value.push_back(std::move(value));
			ignoreWhitespace();
		}
		next();

		return list;
	}

	ValuePtr parseInt() {
		size_t checkpoint = index;	
		while(index < sv.size() && (std::isdigit(sv[index]) || sv[index] == '-')) {
			next();
		}

		if(checkpoint == index) {
			return nullptr;
		}

		int64_t result;
		auto charsResult = std::from_chars(sv.data() + checkpoint, sv.data() + index, result);
		if(charsResult.ec != std::errc()) {
			return nullptr;
		}

		return std::make_unique<IntValue>(result);
	};

	std::string_view parseString() {
		if(!has('"')) {
			return {};
		}

		next();
		size_t checkpoint = index;

		while(!has('"')) {
			next();
			if(index >= sv.size()) {
				return {};
			}
		}

		std::string_view result(
			sv.data() + checkpoint,
			index - checkpoint);
		next();
		return result;
	}

	ValuePtr parseBool() {
		auto trueLen = 4;
		auto falseLen = 5;
		if(index + trueLen >= sv.size()) {
			return nullptr;
		}

		std::string_view view(sv.data() + index, trueLen);
		if(view == "true") {
			index += trueLen;
			return std::make_unique<BoolValue>(true);
		}

		if(index + falseLen >= sv.size()) {
			return nullptr;
		}

		view = std::string_view(sv.data() + index, falseLen);
		if(view == "false") {
			index += falseLen;
			return std::make_unique<BoolValue>(false);
		}
		return nullptr;
	}

	ValuePtr parseFloat() {
		size_t checkpoint = index;

		// seek while digit
		while(index < sv.size() && std::isdigit(sv[index]) || sv[index] == '-') {
			next();
		}

		// none found; bail
		if(checkpoint == index) {
			return nullptr;
		}

		if(!has('.')) {
			index = checkpoint;
			return nullptr; // no dot = not float
		}
		next();

		// seek while digit
		while(index < sv.size() && std::isdigit(sv[index])) {
			next();
		}

		if(has('e') || has('E')) {	// Exp boys
			next();
			if(has('-') || has('+')) {
				next();
			}
			// seek while digit
			while(index < sv.size() && std::isdigit(sv[index])) {
				next();
			}
		} 
		double result;
		auto charsResult = std::from_chars(sv.data() + checkpoint, sv.data() + index, result);
		if(charsResult.ec != std::errc()) {
			return nullptr;
		} 
		return std::make_unique<FloatValue>(result);
	}

	ValuePtr parseNull() {
		const auto nullLen = 4;
		std::string_view view(sv.data() + index, nullLen);
		if(view == "null") {
			index += nullLen;
			return std::make_unique<NullValue>();
		}
		return nullptr;
	}

	bool has(char c) {
		return index < sv.size() && c == sv[index];
	}

	void next() {
		++index;
	}

	void ignoreWhitespace() {
		while(index < sv.size() && std::isspace(sv[index])) {
			next();
		}
	}

	Object* activeObject = nullptr;
	std::string_view sv;
	size_t index = 0;
};

};
