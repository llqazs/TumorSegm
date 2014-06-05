#pragma once

class GraphBuildor
{
public:
	GraphBuildor(CData& d); // data passed to be build, still need to pass the graph
	virtual ~GraphBuildor();

private:
	virtual void BuildHardConstraint();
	virtual void BuildDataTerm();
	virtual void BuildSmoothTerm();
	virtual void BuildStarShape();

private:
	CData& m_data;

};