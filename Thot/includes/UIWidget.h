#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <assert.h>

#include "MathTools.h"

#include "SFML/Graphics.hpp"

namespace Thot {

struct InputEvent;
class UIManager;
class UIContainer;

class UIWidget : public std::enable_shared_from_this<UIWidget>, public sf::Drawable
{
public:
	enum ShapeProcessing { FIXED = 1 << 0, RELATIVE = 1 << 1};
	enum ShapeAxis { X = 1 << 0, Y = 1 << 1, X_Y = (X | Y) };
	enum ShapeTransformation { POSITION = 1<<0, SIZE = 1<<1, POSITION_SIZE = (POSITION | SIZE) };

protected:
	sf::Vector2f m_position;
	sf::Vector2f m_relativePosition; //position [0, 1] relative to parent
	sf::Vector2f m_size;
	sf::Vector2f m_relativeSize; //size [0, 1] relative to parent
	UIManager* m_manager;
	UIWidget* m_parent;
	bool m_isDirty; //need to call UpdateShape() ?
	std::vector<std::shared_ptr<UIWidget>> m_widgets;
	ShapeProcessing m_resizingProcess_x;
	ShapeProcessing m_resizingProcess_y;
	ShapeProcessing m_repositionningProcess_x;
	ShapeProcessing m_repositionningProcess_y;
	//ShapeProcessing m_repositionningMode;
	//ShapeProcessing m_resizingMode;
	sf::Vector2f m_pixelOffset;
	sf::Vector2f m_clampedOrigin;
	sf::Vector2f m_origin;

	int m_drawingIndex; // TODO : Remove ? 

public:
	UIWidget(UIManager* manager);
	virtual ~UIWidget();

	virtual bool OnInputEvent(InputEvent& input);
	virtual bool Contains(float x, float y);
	void UpdateShape();
	void setDirty();
	bool HasParent() const;
	UIWidget* GetParent();

	void AttachToParent(UIWidget* parent, bool childPositionIsRelative = false, bool childSizeRelativeToParent = false);
	virtual void AddChild(std::shared_ptr<UIWidget> widget, bool childPositionIsRelative = false, bool childSizeRelativeToParent = false);
	void AddChildAtomic(std::shared_ptr<UIWidget> child);
	void SetUpFromParent(ShapeAxis axis = ShapeAxis::X_Y, ShapeTransformation transformation = ShapeTransformation::POSITION_SIZE);

	void RemoveFromParent();
	virtual void RemoveChild(std::shared_ptr<UIWidget> widget);
	virtual void RemoveChild(UIWidget* widget);

	void SetPixelSize(float width, float height, bool updateChilds = true);
	void SetPosition(float x, float y, bool updateChilds = true);
	void SetPosition(const UIWidget* anchor, float x, float y, bool updateChilds = true);
	void SetRelativePosition(const UIWidget* anchor, float x, float y, bool updateChilds = true);
	void SetRelativeSize(const UIWidget* anchor, float x, float y, bool updateChilds = true);
	void SetPixelOffset(float x, float y, bool updateChilds = true);
	void SetOrigin(float clampedX, float clampedY, bool updateChilds = true);

	sf::Vector2f GetSize() const;
	sf::Vector2f GetPosition() const;
	sf::Vector2f GetPixelOffset() const;
	sf::Vector2f GetOrigin() const;

	int GetDrawingIndex() const;
	virtual void PrepareToMoveChildOnTop(UIWidget* child);
	virtual void ProcessInternalSorting();

	virtual void UpdateVisual();
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	void OnChapeChanged(bool updateChilds);

	void SetResizingProcess(ShapeProcessing processingMode, ShapeAxis axis = ShapeAxis::X_Y);
	void SetRepositionningProcess(ShapeProcessing processingMode, ShapeAxis axis = ShapeAxis::X_Y);
	ShapeProcessing GetResizingProcess(ShapeAxis axis) const;
	ShapeProcessing GetRepositionningProcess(ShapeAxis axis) const;
	//void SetRepositionningMode(ShapeProcessing mode);
	//void SetResizingMode(ShapeProcessing mode);
	//ShapeProcessing GetRepositionningMode() const;
	//ShapeProcessing GetResizingMode() const;

protected:
	virtual void ProtectedUpdateShape();

};

}