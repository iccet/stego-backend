#include "containerevent.hpp"

ContainerEvent::ContainerEvent(kafka::Topic topic, const kafka::Value& data)
	: QEvent(QEvent::User)
	, Topic(std::move(topic))
    , Data(data)
{

}
