#ifndef DRIVERS_INTERRUPT_HPP
#define DRIVERS_INTERRUPT_HPP

#include <utility>
#include <asm/regs.h>

namespace artemis
{
enum InterruptTriggerMode : bool
{
	IntTriggerModeEdge,
	IntTriggerModeLevel,
};

enum InterruptPolarity : bool
{
	IntPolarityActiveHigh,
	IntPolarityActiveLow,
};

class Interrupt
{
  public:
	typedef void (*interrupt_handler_t)(Iframe* iframe);

	Interrupt() = default;

	Interrupt(const Interrupt&) = delete;
	Interrupt(Interrupt&&) = delete;

	Interrupt& operator=(const Interrupt&) = delete;
	Interrupt& operator=(Interrupt&&) = delete;

	~Interrupt() = default;

	bool is_reserved() const;
	std::uint32_t get_vector() const;
	InterruptPolarity get_polarity() const;
	InterruptTriggerMode get_trigger_mode() const;

	bool reserve();
	void set_polarity(InterruptPolarity polarity);
	void set_vector(std::uint32_t vector);
	void set_trigger_mode(InterruptTriggerMode trigger_mode);

	// template<typename Func, typename... Args>
	// bool Set(Func&& func, Args&&... args)
	// {
	// 	if(this->is_used())
	// 	{
	// 		return false;
	// 	}

	// 	this->handler_ = [func = std::forward<Func>(func),
	// 					  ... args = std::forward<Args>(args)](Iframe* iframe) mutable {
	// 		func(iframe, args...);
	// 	};

	// 	return true;
	// }

	bool Set(interrupt_handler_t handler)
	{
		if(this->is_used())
		{
			return false;
		}

		this->handler_ = handler;

		return true;
	}

    bool reset();
	bool operator()(Iframe* iframe);

  private:
	bool is_used() const
	{
		return bool(this->handler_);
	}

	std::uint32_t vector_;
	InterruptPolarity polarity_;
	InterruptTriggerMode trigger_mode_;
	bool is_reserved_;
	interrupt_handler_t handler_;
};
} // namespace artemis

#endif // DRIVERS_INTERRUPT_HPP