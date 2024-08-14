#include <drivers/interrupt.hpp>

namespace artemis
{
bool Interrupt::is_reserved() const
{
	return this->is_reserved_;
}

std::uint32_t Interrupt::get_vector() const
{
	return this->vector_;
}

InterruptPolarity Interrupt::get_polarity() const
{
	return this->polarity_;
}

InterruptTriggerMode Interrupt::get_trigger_mode() const
{
	return this->trigger_mode_;
}

bool Interrupt::reserve()
{
	if(this->is_reserved_)
	{
		return false;
	}

	return this->is_reserved_ = true;
}

void Interrupt::set_vector(std::uint32_t vector)
{
	this->vector_ = vector;
}

void Interrupt::set_polarity(InterruptPolarity polarity)
{
	this->polarity_ = polarity;
}

void Interrupt::set_trigger_mode(InterruptTriggerMode trigger_mode)
{
	this->trigger_mode_ = trigger_mode;
}

bool Interrupt::reset()
{
	const bool ret = bool(this->handler_);

	this->handler_ = nullptr;
	this->is_reserved_ = false;

	return ret;
}

bool Interrupt::operator()(Iframe* iframe)
{
	if(!this->is_used())
	{
		return false;
	}

	this->handler_(iframe);
	return true;
}
} // namespace artemis