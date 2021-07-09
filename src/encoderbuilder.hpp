#ifndef STEGO_BACKEND_ENCODER_BUILDER_HPP
#define STEGO_BACKEND_ENCODER_BUILDER_HPP

#include <rdkafka.h>

#include <QMetaEnum>

#include <kafka/ConsumerRecord.h>
#include <kafka/Types.h>

#include <stg.hpp>

struct EncoderBuilder
{
	virtual QScopedPointer<Stg::AbstractEncoder> build() = 0;
	virtual ~EncoderBuilder() = default;
};

struct HeaderEncoderBuilder final : public EncoderBuilder
{
	using Headers = QMap<kafka::Header::Key, kafka::Header::Value>;
	HeaderEncoderBuilder() = delete;
	explicit HeaderEncoderBuilder(const Headers &headers);
	~HeaderEncoderBuilder() final = default;

	QScopedPointer<Stg::AbstractEncoder> build() final;

private:
	Stg::Stg::EncoderType _encoderType;

};
#endif //STEGO_BACKEND_ENCODER_BUILDER_HPP
