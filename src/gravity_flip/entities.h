
using namespace war;

// ----------------------------------------------------------------------------

struct e_player : entity
{
	virtual void on_touched( simple_collision::pending_collision& coll ) override;
};
