#ifndef __BITFLAGS_H__
#define __BITFLAGS_H__

#define u8BitsFlags OGE::util::BitFlags<uint8>
#define u16BitsFlags OGE::util::BitFlags<uint16>
#define u32BitsFlags OGE::util::BitFlags<uint32>
#define u64BitsFlags OGE::util::BitFlags<uint64>

namespace OGE
{
	namespace util
	{
		template<class T>
		class BitFlags
		{
		public:
			BitFlags();
			~BitFlags();

			void setFlag(uint8 position, bool value);
			bool testFlag(uint8 position) const;

			void clear();
			T getFlags() const;

		private:
			T flags;
		};

		template<class T>
		BitFlags<T>::BitFlags() : flags(0) {}

		template<class T>
		BitFlags<T>::~BitFlags() {}

		template<class T>
		void BitFlags<T>::setFlag(uint8 position, bool value)
		{
			if(value)
			{
				flags |= (T) 1 << position;
			}								
			else
			{
				flags &= ~ ((T) 1 << position);
			}
		}

		template<class T>
		bool BitFlags<T>::testFlag(uint8 position) const
		{
			return (flags & ((T)1 << position)) == 0 ? false : true;
		}
	
		template<class T>
		void BitFlags<T>::clear()
		{
			flags &= 0;
		}

		template<class T>
		T BitFlags<T>::getFlags() const
		{
			return flags;
		}
	}
}

#endif