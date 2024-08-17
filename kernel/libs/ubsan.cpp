#include <sys/defs.h>
#include <cstdint>
#include <logger.hpp>

#define ABORT_VARIANT(name, params, call)                 \
	__NO_RETURN void __ubsan_handle_##name##_abort params \
	{                                                     \
		__ubsan_handle_##name call;                       \
		__UNREACHABLE();                          \
	}

#define ABORT_VARIANT_VP(name) ABORT_VARIANT(name, (void* a), (a))
#define ABORT_VARIANT_VP_VP(name) ABORT_VARIANT(name, (void* a, void* b), (a, b))
#define ABORT_VARIANT_VP_IP(name) ABORT_VARIANT(name, (void* a, intptr_t b), (a, b))
#define ABORT_VARIANT_VP_VP_VP(name) ABORT_VARIANT(name, (void* a, void* b, void* c), (a, b, c))

namespace artemis
{
namespace ubsan
{
struct SourceLocation
{
	const char* file;
	std::uint32_t line;
	std::uint32_t column;
};

struct TypeDescriptor
{
	std::uint16_t kind;
	std::uint32_t info;
	char* name;
};

struct TypeMismatchInfo
{
	SourceLocation location;
	TypeDescriptor* type;
	std::uintptr_t alignment;
	std::uint8_t type_check_kind;
};

struct ShiftOutOfBoundData
{
	SourceLocation location;
	TypeDescriptor* left_type;
	TypeDescriptor* right_type;
};

struct OverflowData
{
	SourceLocation location;
	TypeDescriptor* type;
};

struct OutOfBoundsData
{
	SourceLocation location;
	TypeDescriptor* array_type;
	TypeDescriptor* index_type;
};

struct UnreachableData
{
	SourceLocation location;
};

struct VlaBoundData
{
	SourceLocation location;
	TypeDescriptor* type;
};

struct FloatCastOverflowData
{
	SourceLocation location;
	TypeDescriptor* from_type;
	TypeDescriptor* to_type;
};

struct InvalidValueData
{
	SourceLocation location;
	TypeDescriptor* type;
};

struct FunctionTypeMismatchData
{
	SourceLocation location;
	TypeDescriptor* type;
};

struct NonNullReturnData
{
	SourceLocation location;
	SourceLocation attr_location;
};

struct NonNullArgData
{
	SourceLocation location;
	SourceLocation attr_location;
};

struct CfiBadIcalldata
{
	SourceLocation location;
	TypeDescriptor* type;
};

SourceLocation unknown_location = {
	"<Unknown Location>",
	0,
	0,
};

__NO_RETURN
void UbsanAbort(const SourceLocation* location, const char* violation)
{
	if(!location || !location->file)
	{
		location = &unknown_location;
	}

	LogFatal("<UBSAN> Filename  = %s", location->file);
	LogFatal("<UBSAN> Line      = %u", location->line);
	LogFatal("<UBSAN> Column    = %u", location->column);
	LogPanic("<UBSAN> Violation = %s", violation);

	__UNREACHABLE();
}

__CDECLS_BEGIN

void __ubsan_handle_type_mismatch(void* data_raw, void* pointer_raw)
{
	TypeMismatchInfo* data = static_cast<TypeMismatchInfo*>(data_raw);
	std::uintptr_t pointer = reinterpret_cast<std::uintptr_t>(pointer_raw);
	const char* violation = "Type Mismatch";

	if(pointer == 0)
	{
		violation = "null pointer access";
	}
	else if(data->alignment && (pointer & (data->alignment - 1)))
	{
		violation = "unaligned access";
	}

	UbsanAbort(&data->location, violation);
}

ABORT_VARIANT_VP_VP(type_mismatch);

void __ubsan_handle_type_mismatch_v1(void* data_raw, void* pointer_raw)
{
	TypeMismatchInfo* data = static_cast<TypeMismatchInfo*>(data_raw);
	std::uintptr_t pointer = reinterpret_cast<std::uintptr_t>(pointer_raw);
	const char* violation = "Type Mismatch";

	if(pointer == 0)
	{
		violation = "null pointer access";
	}
	else if(data->alignment && (pointer & (data->alignment - 1)))
	{
		violation = "unaligned access";
	}

	UbsanAbort(&data->location, violation);
}

ABORT_VARIANT_VP_VP(type_mismatch_v1);

void __ubsan_handle_add_overflow(void* data_raw, void* lhs_raw, void* rhs_raw)
{
	OverflowData* data = static_cast<OverflowData*>(data_raw);
	std::uintptr_t lhs = reinterpret_cast<std::uintptr_t>(lhs_raw);
	std::uintptr_t rhs = reinterpret_cast<std::uintptr_t>(rhs_raw);

	static_cast<void>(lhs);
	static_cast<void>(rhs);

	UbsanAbort(&data->location, "addition overflow");
}

ABORT_VARIANT_VP_VP_VP(add_overflow);

void __ubsan_handle_sub_overflow(void* data_raw, void* lhs_raw, void* rhs_raw)
{
	OverflowData* data = static_cast<OverflowData*>(data_raw);
	std::uintptr_t lhs = reinterpret_cast<std::uintptr_t>(lhs_raw);
	std::uintptr_t rhs = reinterpret_cast<std::uintptr_t>(rhs_raw);

	static_cast<void>(lhs);
	static_cast<void>(rhs);

	UbsanAbort(&data->location, "subtraction overflow");
}

ABORT_VARIANT_VP_VP_VP(sub_overflow);

void __ubsan_handle_mul_overflow(void* data_raw, void* lhs_raw, void* rhs_raw)
{
	OverflowData* data = static_cast<OverflowData*>(data_raw);
	std::uintptr_t lhs = reinterpret_cast<std::uintptr_t>(lhs_raw);
	std::uintptr_t rhs = reinterpret_cast<std::uintptr_t>(rhs_raw);

	static_cast<void>(lhs);
	static_cast<void>(rhs);

	UbsanAbort(&data->location, "multiplication overflow");
}

ABORT_VARIANT_VP_VP_VP(mul_overflow);

void __ubsan_handle_negate_overflow(void* data_raw, void* old_value_raw)
{
	OverflowData* data = static_cast<OverflowData*>(data_raw);
	std::uintptr_t old_value = reinterpret_cast<std::uintptr_t>(old_value_raw);

	static_cast<void>(old_value);

	UbsanAbort(&data->location, "negation overflow");
}

ABORT_VARIANT_VP_VP(negate_overflow);

void __ubsan_handle_divrem_overflow(void* data_raw, void* lhs_raw, void* rhs_raw)
{
	OverflowData* data = static_cast<OverflowData*>(data_raw);
	std::uintptr_t lhs = reinterpret_cast<std::uintptr_t>(lhs_raw);
	std::uintptr_t rhs = reinterpret_cast<std::uintptr_t>(rhs_raw);

	static_cast<void>(lhs);
	static_cast<void>(rhs);

	UbsanAbort(&data->location, "division remainder overflow");
}

ABORT_VARIANT_VP_VP_VP(divrem_overflow);

void __ubsan_handle_pointer_overflow(void* data_raw, void* lhs_raw, void* rhs_raw)
{
	OverflowData* data = static_cast<OverflowData*>(data_raw);
	std::uintptr_t lhs = reinterpret_cast<std::uintptr_t>(lhs_raw);
	std::uintptr_t rhs = reinterpret_cast<std::uintptr_t>(rhs_raw);

	static_cast<void>(lhs);
	static_cast<void>(rhs);

	UbsanAbort(&data->location, "pointer overflow");
}

ABORT_VARIANT_VP_VP_VP(pointer_overflow);

void __ubsan_handle_shift_out_of_bounds(void* data_raw, void* lhs_raw, void* rhs_raw)
{
	ShiftOutOfBoundData* data = static_cast<ShiftOutOfBoundData*>(data_raw);
	std::uintptr_t lhs = reinterpret_cast<std::uintptr_t>(lhs_raw);
	std::uintptr_t rhs = reinterpret_cast<std::uintptr_t>(rhs_raw);

	static_cast<void>(lhs);
	static_cast<void>(rhs);

	UbsanAbort(&data->location, "shift out of bounds");
}

ABORT_VARIANT_VP_VP_VP(shift_out_of_bounds);

void __ubsan_handle_out_of_bounds(void* data_raw, void* index_raw)
{
	OutOfBoundsData* data = static_cast<OutOfBoundsData*>(data_raw);
	std::uintptr_t index = reinterpret_cast<std::uintptr_t>(index_raw);

	static_cast<void>(index);

	UbsanAbort(&data->location, "out of bounds");
}

ABORT_VARIANT_VP_VP(out_of_bounds);

__NO_RETURN
void __ubsan_handle_builtin_unreachable(void* data_raw)
{
	UnreachableData* data = static_cast<UnreachableData*>(data_raw);
	UbsanAbort(&data->location, "reached unreachable");
}

__NO_RETURN
void __ubsan_handle_missing_return(void* data_raw)
{
	UnreachableData* data = static_cast<UnreachableData*>(data_raw);
	UbsanAbort(&data->location, "missing return");
}

void __ubsan_handle_vla_bound_not_positive(void* data_raw, void* bound_raw)
{
	VlaBoundData* data = static_cast<VlaBoundData*>(data_raw);
	std::uintptr_t bound = reinterpret_cast<std::uintptr_t>(bound_raw);

	static_cast<void>(bound);

	UbsanAbort(&data->location, "negative variable array length");
}

ABORT_VARIANT_VP_VP(vla_bound_not_positive);

void __ubsan_handle_float_cast_overflow(void* data_raw, void* from_raw)
{
	FloatCastOverflowData* data = static_cast<FloatCastOverflowData*>(data_raw);
	std::uintptr_t from = std::uintptr_t(from_raw);
	(void)from;
	UbsanAbort(&data->location, "float cast overflow");
}

ABORT_VARIANT_VP_VP(float_cast_overflow);

void __ubsan_handle_load_invalid_value(void* data_raw, void* value_raw)
{
	InvalidValueData* data = static_cast<InvalidValueData*>(data_raw);
	std::uintptr_t value = reinterpret_cast<std::uintptr_t>(value_raw);

	static_cast<void>(value);

	UbsanAbort(&data->location, "invalid value load");
}

ABORT_VARIANT_VP_VP(load_invalid_value);

void __ubsan_handle_function_type_mismatch(void* data_raw, void* value_raw)
{
	FunctionTypeMismatchData* data = static_cast<FunctionTypeMismatchData*>(data_raw);
	std::uintptr_t value = reinterpret_cast<std::uintptr_t>(value_raw);

	static_cast<void>(value);

	UbsanAbort(&data->location, "function type mismatch");
}

ABORT_VARIANT_VP_VP(function_type_mismatch);

void __ubsan_handle_nonnull_return(void* data_raw)
{
	NonNullReturnData* data = static_cast<NonNullReturnData*>(data_raw);
	UbsanAbort(&data->location, "null return");
}

ABORT_VARIANT_VP(nonnull_return);

void __ubsan_handle_nonnull_arg(void* data_raw, std::intptr_t index_raw)
{
	NonNullArgData* data = static_cast<NonNullArgData*>(data_raw);
	std::uintptr_t index = static_cast<std::uintptr_t>(index_raw);

	static_cast<void>(index);

	UbsanAbort(&data->location, "null argument");
}

ABORT_VARIANT_VP_IP(nonnull_arg);

void __ubsan_handle_cfi_bad_icall(void* data_raw, void* value_raw)
{
	CfiBadIcalldata* data = static_cast<CfiBadIcalldata*>(data_raw);
	std::uintptr_t value = reinterpret_cast<std::uintptr_t>(value_raw);

	static_cast<void>(value);

	UbsanAbort(&data->location, "control flow integrity check failure during indirect call");
}

ABORT_VARIANT_VP_VP(cfi_bad_icall);

__CDECLS_END
} // namespace ubsan
} // namespace artemis