<img width="998" height="850" alt="Screenshot 2026-10-18 223645" src="https://github.com/user-attachments/assets/21af1bee-faaa-4d92-8c5e-5d0b85d30377" />

# How do I approach designing software?
I approach designing software by first understanding what the program needs to do and who will use it.  I identify the required functionality, break the larger problems into smaller components, and plan how those components will work together.  Depending on the project, I create design artifacts such as pseudocode, flowcharts, class diagrams, and use case diagrams to work through the logic.  I also apply design patterns when they provide a useful structure.  When performance matters, I consider the time and space complexity of my algorithms using Big O notation.  


### - What new design skills has your work on the project helped you to craft?
I strengthened my ability to use math as a design tool.  I used coordinates and transformations to plan each object's shape and placement, then used projection, texture coordinates, and lighting calculations to refine how the scene appeared from different views.

### - What design process did you follow for your project work?
I began by studying the provided C++ and OpenGL codebase to understand how its existing parts worked together.  I examined how SceneManager prepared and rendered shapes, how ViewManager controlled the camera, and how the shaders received information about each object.  
<img width="1740" height="1003" alt="CS330_StarterCodeUML" src="https://github.com/user-attachments/assets/02d16dca-3e18-498e-8628-a5b98b623eb0" />
Once I understood that foundation, I studied my reference image and broke its tabletop arrangement into basic 3D shapes that I could build with the available meshes.  I began with the wine bottle because it was the most complex object.  It required combining several basic meshes to create its body, shoulder, neck, and lid.  As the milestones progressed, I added camera controls, textures, materials, and lighting.  Then I applied what I learned to build the other objects in the scene.  I added features in manageable steps, recompiling after each change.  Seeing the result helped me decide what to adjust next.  This iterative approach let me develop the scene while checking that each new change worked with what I had already built.  

### - How could tactics from your design approach be applied in future work?
In future projects, I will have experience in studying existing code, analyzing requirements, and breaking work into smaller components.  I will be able to continue using diagrams and pseudocode to plan how those components interact before implementing them.  Building and checking one feature at a time allows me to catch problems early.  Reusable functions and clear organiation make later changes easier.

# How do I approach developing programs?
As I develop a program, I take an iterative approach by testing each small change as I go.  The type of testing depends on the project.  I often run the program to check its behavior, revisit existing features after changes, and evaluate usability and performance.  When a project calls for more structured testing, I may also use unit, integration, or system testing.  Before delivering a program, I review it against the requirements, complete a final test, and provide documentation and files needed to run it.  If I receive feedback or find a problem later, I use that to guide the next revision.

### - What new development strategies did you use while working on your 3D scene?
While developing my 3D scene, I used a reusable object structure to keep each object's mesh, position, scale, color, texture, and material information together.  I moved drawing code into smaller functions and used name constants and texture tags instead of repeating values throughout RenderScene().  These changes made it easier to add objects, adjust their appearance, and track down problems when the rendered result was not what I expected.  

### - How did iteration factor into your development?
Iteration was how I judged whether a change worked.  I built and ran the scene after adding or adjusting objects, camera controls, textures, and lights.  Then I compared the result with my reference image and viewed it from different angles.  Some changes didn't behave as I anticipated.  If I had changed several factors at once, it would have been much more difficult to know what worked and what was causing problems.

### - How has your approach to developing code evolved throughout the milestones, which led you to the project’s completion?
In the early milestones, I focused on constructing and positioning shapes.  As I added camera controls, textures, and lighting, I had to think more about how each new feature affected the whole scene.  After I constructed a basic scene, I incorporated named constants, grouping object information, and moved drawing code into smaller functions.

# How can computer science help me in reaching my goals?

### - How do computational graphics and visualizations give you new knowledge and skills that can be applied in your future educational pathway?
Computational graphis helped me connect programming with the math I study in my applied mathematics minor.  I used vectors, coordinates, transformations, and angles to build and navigate a 3D scene.  I was able to see how those calculations affected the image on screen.  I look forward to building this skill in future courses involving mathematics and data visualization.  I plan to apply what I learned into my side projects at home working with sensors and updating my NASA 2D LiDAR display.

### - How do computational graphics and visualizations give you new knowledge and skills that can be applied in your future professional pathway?
Computational graphics can help me become a stronger front-end developer.  I now have a better understanding of how shapes, color, lighting, perspective, and user input affect what people see and how they interact with a screen.  I could use that knowledge to create more engaging interfaces, interactive diagrams, product views, or visualizations that users can explore.  This project also taught me to consider performance and usability while developing visual features so the graphics support the user's experience.
