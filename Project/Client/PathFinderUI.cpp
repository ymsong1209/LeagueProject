#include "pch.h"
#include "PathFinderUI.h"

int PathFinderUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

}

PathFinderUI::PathFinderUI()
	: ComponentUI("##PathFinder", COMPONENT_TYPE::PATHFINDER)
{
	SetName("PathFinder");
}

PathFinderUI::~PathFinderUI()
{
}
