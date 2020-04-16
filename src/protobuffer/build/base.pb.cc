// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: base.proto

#include "base.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)

namespace Base {
class ConnectionDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<Connection>
      _instance;
} _Connection_default_instance_;
}  // namespace Base
namespace protobuf_base_2eproto {
static void InitDefaultsConnection() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::Base::_Connection_default_instance_;
    new (ptr) ::Base::Connection();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::Base::Connection::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_Connection =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsConnection}, {}};

void InitDefaults() {
  ::google::protobuf::internal::InitSCC(&scc_info_Connection.base);
}

::google::protobuf::Metadata file_level_metadata[1];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Base::Connection, _has_bits_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Base::Connection, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Base::Connection, nickname_),
  0,
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 6, sizeof(::Base::Connection)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::Base::_Connection_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  AssignDescriptors(
      "base.proto", schemas, file_default_instances, TableStruct::offsets,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\nbase.proto\022\004Base\"\036\n\nConnection\022\020\n\010nick"
      "name\030\001 \002(\t"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 50);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "base.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_base_2eproto
namespace Base {

// ===================================================================

void Connection::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Connection::kNicknameFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Connection::Connection()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_base_2eproto::scc_info_Connection.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:Base.Connection)
}
Connection::Connection(const Connection& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  nickname_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.has_nickname()) {
    nickname_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.nickname_);
  }
  // @@protoc_insertion_point(copy_constructor:Base.Connection)
}

void Connection::SharedCtor() {
  nickname_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

Connection::~Connection() {
  // @@protoc_insertion_point(destructor:Base.Connection)
  SharedDtor();
}

void Connection::SharedDtor() {
  nickname_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void Connection::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ::google::protobuf::Descriptor* Connection::descriptor() {
  ::protobuf_base_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_base_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const Connection& Connection::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_base_2eproto::scc_info_Connection.base);
  return *internal_default_instance();
}


void Connection::Clear() {
// @@protoc_insertion_point(message_clear_start:Base.Connection)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    nickname_.ClearNonDefaultToEmptyNoArena();
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool Connection::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:Base.Connection)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string nickname = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_nickname()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->nickname().data(), static_cast<int>(this->nickname().length()),
            ::google::protobuf::internal::WireFormat::PARSE,
            "Base.Connection.nickname");
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:Base.Connection)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:Base.Connection)
  return false;
#undef DO_
}

void Connection::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:Base.Connection)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required string nickname = 1;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->nickname().data(), static_cast<int>(this->nickname().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "Base.Connection.nickname");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->nickname(), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:Base.Connection)
}

::google::protobuf::uint8* Connection::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:Base.Connection)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required string nickname = 1;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->nickname().data(), static_cast<int>(this->nickname().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "Base.Connection.nickname");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->nickname(), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Base.Connection)
  return target;
}

size_t Connection::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Base.Connection)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  // required string nickname = 1;
  if (has_nickname()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->nickname());
  }
  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Connection::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:Base.Connection)
  GOOGLE_DCHECK_NE(&from, this);
  const Connection* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Connection>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:Base.Connection)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:Base.Connection)
    MergeFrom(*source);
  }
}

void Connection::MergeFrom(const Connection& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:Base.Connection)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.has_nickname()) {
    set_has_nickname();
    nickname_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.nickname_);
  }
}

void Connection::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:Base.Connection)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Connection::CopyFrom(const Connection& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Base.Connection)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Connection::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  return true;
}

void Connection::Swap(Connection* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Connection::InternalSwap(Connection* other) {
  using std::swap;
  nickname_.Swap(&other->nickname_, &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::google::protobuf::Metadata Connection::GetMetadata() const {
  protobuf_base_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_base_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace Base
namespace google {
namespace protobuf {
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::Base::Connection* Arena::CreateMaybeMessage< ::Base::Connection >(Arena* arena) {
  return Arena::CreateInternal< ::Base::Connection >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
