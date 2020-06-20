#pragma once

struct w_layer : i_listener, i_lifecycle
{
	w_layer();

	/*
		called each time the layer is pushed onto the stack
	
		good for setting state variables and making sure the state
		is ready to start updating/drawing
	*/
	virtual void push() {}

	/*
		called each time the layer is removed from the stack
	*/
	virtual void pop() {}

	/*
		called when this layer is becoming the top layer.

		-	when it is initially pushed onto the stack
		-	when the layer above it is popped off and this
			becomes the topmost layer
	*/
	virtual void becoming_top_layer() {}

	/*
		called when this layer is being getting_covered up by another layer
	*/
	virtual void getting_covered() {}

	virtual void update() {}
	virtual void draw();
	virtual e_opaque get_opaque_flags();
};
