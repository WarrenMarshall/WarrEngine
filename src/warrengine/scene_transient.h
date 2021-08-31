
namespace war
{

// an empty scene that does nothing other than block user input and scene
// updates. used to stop the user from interacting with the engine while we're
// doing something like traansitioning between scenes.

struct Scene_Transient final : scene
{
	Scene_Transient();
};

}
