#ifndef LIBS_BITMAP_HPP
#define LIBS_BITMAP_HPP

#include <concepts>
#include <sys/defs.h>

namespace artemis
{
enum BitmapStatus : bool
{
	BitmapAvailable,
	BitmapUsed,
};

template<std::unsigned_integral T>
class Bitmap
{
  public:
	constexpr Bitmap() = default;

	constexpr Bitmap(T* data) : data_(data), size_(0)
	{
	}

	constexpr Bitmap(T* data, std::size_t size) : data_(data), size_(size)
	{
	}

	Bitmap(const Bitmap&) = delete;
	Bitmap(Bitmap&&) = delete;

	Bitmap& operator=(const Bitmap&) = delete;
	Bitmap& operator=(Bitmap&&) = delete;

	~Bitmap() = default;

	void Initialize(T* data, std::size_t size)
	{
		this->data_ = data;
		this->size_ = size;
	}

	constexpr T Byte(std::size_t index) const
	{
		return this->data_[index / bits_in_type];
	}

	constexpr bool Bit(std::size_t index) const
	{
		return this->Byte(index) & (1 << (index % bits_in_type));
	}

	constexpr bool operator[](std::size_t index) const
	{
		return this->Bit(index);
	}

	constexpr void Set(std::size_t index, bool value)
	{
		if(value)
		{
			this->data_[index / bits_in_type] |= (static_cast<T>(1) << (index % bits_in_type));
		}
		else
		{
			this->data_[index / bits_in_type] &= ~(static_cast<T>(1) << (index % bits_in_type));
		}
	}

	constexpr void Fill(bool value)
	{
		for(std::size_t i = 0; i < size_; i++)
		{
			this->Set(i, value);
		}
	}

  private:
	constexpr static int bits_in_type = (sizeof(T) * 8);
	T* data_;
	std::size_t size_;
};
} // namespace artemis

#endif // LIBS_BITMAP_HPP