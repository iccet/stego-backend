#include "encoderbuilder.hpp"

HeaderEncoderBuilder::HeaderEncoderBuilder(const Headers &headers)
	: _encoderType(Stg::Stg::EncoderType::Default)
{
	auto meta = QMetaEnum::fromType<Stg::Stg::EncoderType>();
	auto header = headers.value(meta.enumName());
	_encoderType = (Stg::Stg::EncoderType)meta.keyToValue(static_cast<const char*>(header.data()));
}

QScopedPointer<Stg::AbstractEncoder> HeaderEncoderBuilder::build()
{
	switch(_encoderType)
	{
		case Stg::Stg::EncoderType::Lsb:
			return QScopedPointer<Stg::AbstractEncoder>(new Stg::Lsb());
		case Stg::Stg::EncoderType::Kutter:
			return QScopedPointer<Stg::AbstractEncoder>(new Stg::Kutter());
		default:
			Q_UNREACHABLE();
	}
}
