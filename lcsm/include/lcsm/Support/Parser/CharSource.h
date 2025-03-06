#ifndef LCSM_SUPPORT_PARSER_CHARSOURCE_H
#define LCSM_SUPPORT_PARSER_CHARSOURCE_H

namespace lcsm
{
	namespace support
	{
		class CharSource
		{
		  public:
			CharSource() noexcept = default;
			virtual ~CharSource() noexcept = default;

			virtual bool hasNext() = 0;
			virtual char next() = 0;

			static constexpr char EndOfSource = '\0';
		};
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_PARSER_CHARSOURCE_H */
