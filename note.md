# Current Design Notes 
Design notes for the forseeable render scene system overhaul to 
fully utilize Entity Component System (ECS) Cache friendly data storage
and isolate inner Scene graph mechanisms for efficient scene traversals

***

## Major Updates 
### Memory Management
    - [ ] buffer managment and provision by ResourceManager
    - [ ] buffer storage device polymorphism 
    - [ ] 

### Scene Rendering and traversals
    - [ ] Upon registered component changes, send to vector for  
          processing sorted by scene tree layer and update 
          per frame, progressing through children nodes (if applicable)

    - [ ] Create draw calls by iterating through sceneList of nodes, 

    - [ ] add a "layer" attribute to each scene node
    - [ ] polymorphise scene nodes to simplify node traversal logic 

> Model instances have a scene graph called instanceGraph that can have nodes that 
> are meshes, cameras, etc, etc, and can have children. 
> For example, a node that is a child of the model root
> like the head, could have a child node being the camera
> then the camera could follow the head, since its all entities
> so traversing a scene graph and model graph requires 
> seperate functions that handle each type of node. 
> it might be a good idea to change scene nodes to handle 
> polymorphism (perhaps crtp) and include an (update) function 



Plan: 


Scene graph are only used to update transforms, bounding boxes 
and other related issues. When one wishes to create a new instance, the instance is added 
by first, creating shared pointers to the relevant data (transforms and whatnot) 
and attaching them to the new scene node, a shared pointer to the node should be
stored in the component manager as a component attached to the enitity.
The scene node is then inserted as a weak pointer child to either the root scene node
or the parent node that is selected, whichever applies, if the root is the parent, 
make the layer equal to 1, if the root is the parent 

Every time the component manager gets a successful request to change a component, 
the component id and the entity id is sent to a changes vector in whichever manager manages 
the scene and this is sorted by the layer attribute in the scene node. 
When its time to handle any changes for whatever relevant system (transforms, collisions, etc)
the relevant system queries the changes from the component manager and traverses the scene 
from each node highest to lowest 

ResourceManager provides a managed buffer of preallocated data that is named for a particular use
one particular use is the storage of mesh data for rendering, buffers can be paginated, return 
an enum value of FULL, EMPTY, or ERROR, when attempting to insert into a buffer, if FULL, create a new buffer 
at the index matching the page (pushback buffer wrapper object pointer into a cpu side vector in resource manager)
and insert the mesh data into a new buffer "Page" across render calls, draw calls are created, and seperated, by page number
and a batch container of draw calls will be sent to Renderer with the draw call data and buffers to select.
When a model path is given to load a model, the model offers up all its meshes and one by one 
they are loaded into the given buffer, each time, the mesh vertex (and index) size and offset will be recorded along 
with the mesh index 


Draw call generation: 
Iterate through all renderables with transforms and meshRecord
for each entity, get the index and vertex size and offset in buffer (in meshRecord component)
push back instance data
