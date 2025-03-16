#ifndef LCSM_SUPPORT_IO_READER_H
#define LCSM_SUPPORT_IO_READER_H

namespace lcsm
{
	namespace support
	{
		class Reader
		{
		  public:
			Reader() noexcept = default;
			virtual ~Reader() noexcept = default;

			virtual bool hasNext() = 0;
			virtual char next() = 0;

			static constexpr char EndOfSource = '\0';
		};
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_IO_READER_H */
