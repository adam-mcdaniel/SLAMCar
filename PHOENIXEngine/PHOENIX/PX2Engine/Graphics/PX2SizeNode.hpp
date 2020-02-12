// PX2SizeNode.hpp

#ifndef PX2SIZENODE_HPP
#define PX2SIZENODE_HPP

#include "PX2Node.hpp"
#include "PX2Float2.hpp"
#include "PX2Size.hpp"
#include "PX2Rect.hpp"
#include "PX2Function.hpp"
#include "PX2InputEventData.hpp"

namespace PX2
{
	enum SizeNodePickType
	{
		SNPT_WIDGET_PICKED,
		SNPT_SIZENODE_PICKED,
		SNPT_SIZENODE_NOTPICKED,
		SNPT_MAX_TYPE
	};

	class PX2_ENGINE_ITEM SizeNode : public Node
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(SizeNode);

	public:
		SizeNode();
		virtual ~SizeNode();

		void UpdateLayout();
		void UpdateScreenRect();

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		virtual void OnBeAttached();
		virtual void OnBeDetach();

		// Size
	public:
		void SetSize(float width, float height);
		void SetSize(const Sizef &size);
		const Sizef &GetSize() const;
		void SetWidth(float width);
		float GetWidth() const;
		void SetHeight(float height);
		float GetHeight() const;

		void SetScriptHandlerSizeChanged(const std::string &scriptHandler);
		const std::string &GetScriptSizeChanged() const;

		virtual void OnSizeChanged();

		Rectf GetLocalRect() const;
		Rectf GetWorldRect() const;
		APoint WorldPosToViewPortPos(const APoint &screenPos);
		bool IsInSizeRange(const SizeNode *node) const;
		bool IsIntersectSizeRange(const SizeNode *node) const;

		typedef void (SizeNode::*SizeChangeCallback)(SizeNode *tellObject);
		void SetSizeChangeCallback(SizeNode *object, SizeChangeCallback callback);

	protected:
		Sizef mSize;

		SizeChangeCallback mSizeChangeCallback;
		SizeNode *mSizeChangeTellToObject;
		std::string mScriptHandlerSizeChanged;

		// Pvoit
	public:
		void SetPivot(float x, float y);
		void SetPivot(const Float2 &pvoit);
		const Float2 &GetPivot() const;

		virtual void OnPivotChanged();

	protected:
		Float2 mPivot;

		// Anchor Layout
	public:
		void EnableAnchorLayout(bool enable);
		bool IsAnchorLayoutEnable() const;

		void SetAnchorHor(float anchorX, float anchorY);
		void SetAnchorHor(const Float2 &anchor);
		const Float2 &GetAnchorHor() const;
		void SetAnchorParamHor(float param0, float param1);
		void SetAnchorParamHor(const Float2 &param);
		const Float2 &GetAnchorParamHor() const;

		void SetAnchorVer(float anchorX, float anchorY);
		void SetAnchorVer(const Float2 &anchor);
		const Float2 &GetAnchorVer() const;
		void SetAnchorParamVer(float param0, float param1);
		void SetAnchorParamVer(const Float2 &param);
		const Float2 &GetAnchorParamVer() const;

	public_internal:
		void _MarkRelatvieLayoutChange();
		void _MarkRelatvieScreenChange();
		AVector LeftBottomCornerOffset;

	protected:
		bool mIsAnchorLayoutEnable;
		Float2 mAnchorHor;
		Float2 mAnchorVer;
		Float2 mAnchorParamHor;
		Float2 mAnchorParamVer;

		// ScreenRect
	public:
		void EnableScreenRectLayout(bool enable);
		bool IsEnableScreenRectLayout() const;

		virtual void SetScreenRect(const Rectf &rect);
		const Rectf &GetScreenRect() const;

	protected:
		bool mIsEnableScreenRectLayout;
		Rectf mScreenRect;

	public_internal:
		virtual void UpdateLayout(Movable *parent);
		virtual void UpdateLeftBottomCornerOffset(Movable *parent);
		virtual void UpdateScreenRect(Movable *parent);

	protected:
		bool mIsLayoutChanged;
		bool mIsScreenRectChanged;

		// pick
	public:
		void SetWidget(bool isWidget);
		bool IsWidget() const;

		void SetSelfPickSizeExtend(const Float2 &extend);
		const Float2 &GetSelfPickSizeExtend() const;

		void SetChildPickOnlyInSizeRange(bool onlyInRange);
		bool IsChildPickOnlyInSizeRange() const;

		void SetChildNotPickRecursion(bool notPickRecursion);
		bool IsChildNotPickRecursion() const;

		virtual void PreCanvasPick(const CanvasInputData &inputData, Canvas *canvas);
		virtual void OnWidgetPicked(const CanvasInputData &inputData);
		virtual void OnSizeNodePicked(const CanvasInputData &inputData);
		virtual void OnSizeNodeNotPicked(const CanvasInputData &inputData);
		const CanvasInputData &GetLastPickData() const;

		typedef void (Object::*MemPickCallback)(SizeNode *sizeNode, SizeNodePickType type,
			const CanvasInputData &inputData);
		void SetMemPickCallback(Object *object, MemPickCallback callback);
		MemPickCallback GetMemPickCallback() const;

		void SetScriptHandlerWidgetPicked(const std::string &scriptHandler);
		void SetScriptHandlerNodePicked(const std::string &scriptHandler);
		void SetScriptHandlerNodeNotPicked(const std::string &scriptHandler);
		std::string GetScriptHandlerWidgetPicked() const;
		std::string GetScriptHandlerNodePicked() const;
		std::string GetScriptHandlerNodeNotPicked() const;

	protected:
		bool _IsInRect(const APoint &logicPos);

		bool mIsWidget;
		Float2 mSelfPickSizeExtend;
		bool mIsChildPickOnlyInSizeRange;
		bool mIsNotPickRecursion;
		CanvasInputData mLastPickData;

		std::string mScriptHandlerWidgetPicked;
		std::string mScriptHandlerNodePicked;
		std::string mScriptHandlerNodeNotPicked;

		Object *mMemObject;
		MemPickCallback mMemPickCallback;
	};

	PX2_REGISTER_STREAM(SizeNode);
	typedef Pointer0<SizeNode> SizeNodePtr;
#include "PX2SizeNode.inl"

}

#endif