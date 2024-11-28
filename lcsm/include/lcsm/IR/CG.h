#ifndef LCSM_IR_CG_H
#define LCSM_IR_CG_H

#include <lcsm/Support/PointerView.hpp>

#include <vector>

namespace lcsm
{
	enum CGNodeType : unsigned
	{
		Static,
		Fast,
		Composite,
		Dynamic,
	};

	class CGNode
	{
	  public:
		CGNode(CGNodeType nodeType);

		CGNode(const CGNode &other) = delete;
		CGNode(CGNode &&other) noexcept;

		CGNode &operator=(const CGNode &other) = delete;
		CGNode &operator=(CGNode &&other) noexcept;

		void swap(CGNode &other) noexcept;

		CGNodeType nodeType() const noexcept;

		bool isStatic() const noexcept;
		bool isFast() const noexcept;
		bool isComposite() const noexcept;
		bool isDynamic() const noexcept;

		void pushBackChild(const support::PointerView< CGNode > &child);
		void pushBackChild(support::PointerView< CGNode > &&child);

	  protected:
		CGNodeType m_nodeType;
		std::vector< support::PointerView< CGNode > > m_children;
	};

	class CG
	{
	  public:
		CG() = default;

		CG(const CG &other) = delete;
		CG(CG &&other) noexcept;

		CG &operator=(const CG &other) = delete;
		CG &operator=(CG &&other) noexcept;

		void swap(CG &other) noexcept;

		void addRoot(const support::PointerView< CGNode > &node);
		void addRoot(support::PointerView< CGNode > &&node);

		void resetValues() noexcept;

		const std::vector< support::PointerView< CGNode > > &roots() const noexcept;
		std::vector< support::PointerView< CGNode > > &roots() noexcept;

	  private:
		std::vector< support::PointerView< CGNode > > m_roots;
	};
}	 // namespace lcsm

#endif /* LCSM_IR_CG_H */
