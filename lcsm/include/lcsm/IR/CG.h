#ifndef LCSM_IR_CG_H
#define LCSM_IR_CG_H

#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Instruction.h>
#include <lcsm/Support/PointerView.hpp>

#include <vector>

namespace lcsm
{
	enum CGNodeType : unsigned
	{
		Unknown,
		Static,
		Fast,
		Composite,
		Dynamic,
	};

	class CGStaticNode;
	class CGFastNode;
	class CGCompositeNode;
	class CGDynamicNode;

	class CGNode
	{
	  public:
		CGNode() = default;
		virtual ~CGNode() noexcept = default;

		CGNode(const CGObjectView &target);
		CGNode(CGObjectView &&target);

		CGNode(const CGNode &other) = delete;
		CGNode(CGNode &&other) noexcept;

		CGNode &operator=(const CGNode &other) = delete;
		CGNode &operator=(CGNode &&other) noexcept;

		void swap(CGNode &other) noexcept;

		virtual CGNodeType nodeType() const noexcept;

		bool isStatic() const noexcept;
		bool isFast() const noexcept;
		bool isComposite() const noexcept;
		bool isDynamic() const noexcept;

		virtual CGStaticNode *asStatic() noexcept;
		virtual const CGStaticNode *asStatic() const noexcept;
		virtual CGFastNode *asFast() noexcept;
		virtual const CGFastNode *asFast() const noexcept;
		virtual CGCompositeNode *asComposite() noexcept;
		virtual const CGCompositeNode *asComposite() const noexcept;
		virtual CGDynamicNode *asDynamic() noexcept;
		virtual const CGDynamicNode *asDynamic() const noexcept;

		CGObjectView &target() noexcept;
		const CGObjectView &target() const noexcept;
		void setTarget(const CGObjectView &target) noexcept;
		void setTarget(CGObjectView &&target) noexcept;

		std::vector< support::PointerView< CGNode > > &children() noexcept;
		const std::vector< support::PointerView< CGNode > > &children() const noexcept;
		void pushBackChild(const support::PointerView< CGNode > &child);
		void pushBackChild(support::PointerView< CGNode > &&child);

	  protected:
		CGObjectView m_target;
		std::vector< support::PointerView< CGNode > > m_children;
	};

	class CGStaticNode : public CGNode
	{
	  public:
		CGStaticNode() = default;
		~CGStaticNode() noexcept = default;

		CGStaticNode(const CGObjectView &object);
		CGStaticNode(CGObjectView &&object);
		CGStaticNode(CGObject *object);

		virtual CGNodeType nodeType() const noexcept override final;

		virtual CGStaticNode *asStatic() noexcept override final;
		virtual const CGStaticNode *asStatic() const noexcept override final;
	};

	class CGFastNode : public CGNode
	{
	  public:
		CGFastNode() = default;
		~CGFastNode() noexcept = default;

		CGFastNode(const CGObjectView &object);
		CGFastNode(CGObjectView &&object);
		CGFastNode(CGObject *object);

		virtual CGNodeType nodeType() const noexcept override final;

		virtual CGFastNode *asFast() noexcept override final;
		virtual const CGFastNode *asFast() const noexcept override final;
	};

	class CGCompositeNode : public CGNode
	{
	  public:
		CGCompositeNode() = default;
		~CGCompositeNode() noexcept = default;

		CGCompositeNode(const CGObjectView &object);
		CGCompositeNode(CGObjectView &&object);
		CGCompositeNode(CGObject *object);

		virtual CGNodeType nodeType() const noexcept override final;

		virtual CGCompositeNode *asComposite() noexcept override final;
		virtual const CGCompositeNode *asComposite() const noexcept override final;
	};

	class CGDynamicNode : public CGNode
	{
	  public:
		CGDynamicNode() = default;
		~CGDynamicNode() noexcept = default;

		CGDynamicNode(const CGObjectView &object);
		CGDynamicNode(CGObjectView &&object);
		CGDynamicNode(CGObject *object);

		virtual CGNodeType nodeType() const noexcept override final;

		virtual CGDynamicNode *asDynamic() noexcept override;
		virtual const CGDynamicNode *asDynamic() const noexcept override;
	};

	using CGNodeView = support::PointerView< CGNode >;

	class CG
	{
	  public:
		CG() = default;

		CG(const CG &other) = delete;
		CG(CG &&other) noexcept;

		CG &operator=(const CG &other) = delete;
		CG &operator=(CG &&other) noexcept;

		void swap(CG &other) noexcept;

		void addRoot(const CGNodeView &node);
		void addRoot(CGNodeView &&node);

	  private:
		std::vector< CGNodeView > m_roots;
	};
}	 // namespace lcsm

#endif /* LCSM_IR_CG_H */
