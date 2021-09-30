# 6.837 Lecture Notes

Lecture notes for 6.837 as taught in Fall 2017 by Prof. Justin Solomon.

The official Course Schedule is linked [here](https://docs.google.com/spreadsheets/d/1PmDsw6eXuZCAkLiric2gRZrc2keIzXt-q8WFBsqiTtU/edit#gid=0). In the case that this document gets deleted, you can also check out the lecture videos on Prof. Solomon's [Youtube channel](https://www.youtube.com/user/justinmsolomon).

In these lecture notes, the different formatting represents:

**Important Notes** or **Small Section Headings**

*Personal Comments*

>Quotes from class (either by professor or by other students)

## Lecture 1: Introduction

### What you will learn from this course

- Fundamentals of computer graphics algorithms
- Basics of real-time rendering (w/ OpenGL)
- C/C++ programming experience

### Math used in this class

Linear algebra
- Homogeneous coordinates
- ODEs and their numerical solution
- Sampling, antialiasing (FFTs)

### Administrative stuff

- JS OH: W10-12 (32-D460)
- TA OH: MT7-8 (5th floor of Stud)
- Course Resources are on Stellar
- Q&A Discussion is on Piazza
- Course Schedule is on [Google Docs](https://docs.google.com/spreadsheets/d/1PmDsw6eXuZCAkLiric2gRZrc2keIzXt-q8WFBsqiTtU/edit#gid=0)
- Lecture videos on [Youtube](https://www.youtube.com/user/justinmsolomon)

### Grading Policy

- 50% assignments (biweekly)
- 20% final project - **Danny bae**
	- proposal due **Nov 15th**
	- presentation due **Dec 12th**
	- report due **Dec 13th**
- 20% quiz (nov 16th)
- 10% nanoquizzes (two lowest dropped)

### Assignment details

- Submit Code + executable (athena linux)
- Include README file (metadata of assignment)
- Work on keeping good coding style for your own sanity
- Budget of 3 late days for the semester (beyond that, lose 25% / day) -- Prof. doesn't wanna hear sob stories

### Assignment list

0. Warm up (mesh display w/ OpenGL)
1. Curves & Surfaces
2. Hierachical Modelling, Skinning -- reference frames on steroids
3. Physically-Based Simulation -- lol it means just use video game physics
4. Ray Tracing
5. Real-time graphics

## Lecture 2: Bezier Curves and Splines (revised)

Rendering any object requires us have some sort of representation that can express how the object looks in a scene. Representing something like a triangle and line is fairly straightforward. But anything more complex is hard. Say an object was mapped as a series of points; if we wanted to morph the object, we'd have to remap every point. So, we need a higher level representation for more complex structures. In this lecture we'll look at how curves and splines are good representation that could be used for fairly complex shapes.

Let's look at how we should think about curves. From a **Differential Geometry Standpoint**, a curve is a plane (triangle) / line if viewed locally. Intrisically, a curve is a 1D shape that extrinsically exists on a 2D or 3D space.

The simplest way to represent a curve in a space is: $\vec\gamma(t) = \vec\gamma_0 + t \vec\alpha$

### Modelling Curves

However, this would require us to have a very complex vector in order to represnt $\alpha$ that could represent arbitrary curves. In general, we should consider different qualities in a specific representation of curves. These could include:
- Ease of editing
- Conversion to polyline
- Extensibility to surfaces

**Aside:** Curves are useful for computing inbetween frames in animations.

**Aside:** You can draw curves using a technique called Tesselation where you rasterize line segments into pixels (OpenGL does that for you)

**Key Idea:** Finding a good representation for curves is to find a balance between *expressiveness* and *simplicity*

### Splines

> The term spline comes from ships. -- Prof. Justin Solomon

*...James would be ecstatic to know*

One very good representation of curves are splines. In order to make spline curves, the user specifies control points. And then the rest is interpolated by a smooth curve. This curve should be completely parametrized by the control points. Before explaining more about splines let's look at how curves are formed from a more mathematical perspective.

In general to represent curves we use several **basis functions**: A set of functions $\phi_i(t)$ spanning a set of curves, expressible as $\Sigma_i a_i \phi_i(t)$. E.g. monomial basis.

**Polynomials** could be thought of as **vector spaces** in our context. In the polynomial vector space, $\{1, t, ..., t^n\}$ are the basis vectors, $a_0, a_1, ..., a_n$ of the polynomial.

> Oh Boy -- Prof. Justin Solomon

### Interpolation

Back to splines... When we want to draw a **spline**, we want to be able to control not only do we want the curve to start and end on points we specified but we also want to control the **tangents** at the end points as well. So we want to specify the **location** the start and end as well as their **tangents**.

For a problem with two points and two tangents (4DOF) we need a cubic equation for the unique curve. The problem is now: using these 4 constraints, we need to find the coefficient in the monomial basis so that we are able to draw them.

> Nothing exciting should be happening unless I make it happen. - Prof. Justin Solomon

Let's consider a cubic curve equation in the monomial basis and it's derivative.

$f(t) = at^3 + bt^2 + ct + d$
$f'(t) = 3at^2 + 2bt + c$

We can find the start and end of a curve parametrized in the interval $(0,1)$ and the first order derivatives given constants $a$, $b$, $c$, $d$ of this curve in the Monomial basis:

$f(0) = d$
$f(1) = a+b+c+d$
$f'(0) = c$
$f'(1) = 3a + 2b + c$

We can convert this into matrix form as well. Inverting the result matrix, we can get the transformation matrix for getting the monomial basis from the 4 inputs. This basis where we specify a curve using the two end point locations and their derivatives is called the **Hermite Basis**.

In order to draw a **Hemite Curve** we just parametrize both $x$ and $y$ projections separately with $t$.

### The Bernstein Basis

For practical reasons, the Hermite Basis is not the best way of representing a curve. Instead, an elegant method to sample points on a cubic curve is to use what is called a **Control Polygon**.

In order to understand how **Control Polygons** we will take a quick detour and take a look at **Cubic Blossoms**.

We can transform $f(t) \rightarrow F(t_1, t_2, t_3)$ s.t. this new function $F$ is symmetric, affine, and diagonal.

E.g. $3t_3 - t + 1$ has the cubic blossom:

$F(t_1, t_2, t_3) = 3 t_1 t_2 t_3 - \frac{1}{3} (t_1 + t_2 + t_3) + 1$

We can compute for points from a given cubic blossom at points (0,0,0), (0,0,1), (0,1,1), (1,1,1).

*Refer to lecture notes for more detailed analysis with the math involved. Lecture slides have a very good illustration of how this works.*

Through analysing properties of the **cubic blossom** we essentially have found an easy way of calculating a cubic polynomial very quickly. This method is called the **DeCasteljau Algorithm**. If I were given the cubic blossoms for the four points mentioned above we get the **Bernstein basis**.

### Issues with polynomial representation

An issue with using arbitrary degree polynomials is that it gets too complicated way too easily and it becomes too difficult to keep them from behaving wildly. Higher level polynomials give us uncontrollable high degree polynomial noise. So we use piecewise polynomials to cope: **splines**. To be continued in next lecture...

## Lecture 3: Curves and Surfaces (revised)

### Quick Review: Cubic Control Polygon

Reviewing from last lecture let's take a closer look at the formation of the **Bernstein Basis**. Expressing **DeCasteljau Algorithm** in terms of equations:

Point #1 $= t P_1 + (1 - t) P_0$
Point #2 $= t P_2 + (1 - t) P_1$
Point #3 $= t P_2 + (1 - t) P_3$

Here, points #1, #2 and #3 are $t$ fractions of the three segments created by the 4 control points used in the **Bernstein basis**.

Point #5 $= t [t P_2 + (1 - t) P_1] + (1-t) [t P_1 + (1 - t) P_0]$
Point #6 $= t [t P_3 + (1 - t) P_2] + (1-t) [t P_2 + (1 - t) P_1]$

Similarly, #5 and #6 are the fractions of the two segments created from points #1, #2 and #3.

The final Point #7 created by using points #5 and #6 give us the value of $P(t)$.

$P(t) = (1-t)^3 P_0 + 3 t (1-t)^2 P_1 + 3t^2 (1-t) P_2 + t^3 P_3$

As such, we have rigorously calculated the **Bernstein basis**.

Now, continuing off from last time... a **Spline** is a piecewise polynomial with a high degree of smoothness where pieces meet.

**Aside:** in the 17th century interpolation was **actually** done by using splines: fix the spline at the control points and given time, the shape of the spline fit a smooth curve.

### Differential Properties of Curves

In order to be able to join the cubic curves we've learned to draw using the different basis as well as understand how to compute normals of the surfaces on these curves etc, we need to understand some math.

Curves defined using the **Bernstein Basis** are called **Bezier Curves**. Given a **Bezier curve**, we can get the velocity of traversing the curve by taking its derivative.

$\vec\tau(t) := \frac{P'(t)}{||P'(t)||}$

We can also calculate the curvature vector:

$\vec\kappa(t) := \vec{\dot \tau}(t) \propto \vec{\tau'}(t)$

From vector calculus we know that the $\vec\kappa$ and $\vec\tau$ vectors are orthogonal.

$\vec\kappa \vec\tau = 0$

**Aside**: Take 6.838 for more on this.

**Orders of Continuity**

There are several different orders of continuity defined to illustrate the different classes of continuity that exist. The ones we care about in this class are.

- $C_0$ = continuous: The seam can be a sharp kink
- $G_1$ = geometric continuity: **Tangents align** at the seam
- $C_1$ = parametric continuity: **Same velocity** at the seam
- $C_2$ = curvature continuity: **Tangents** and their derivatives are the same

In this course we'll be mostly talking about $G_1$ and $C_1$.

**Connecting Cubic Bezier Curves** ... is to guarantee...
- $C_0$: $P_4$ of curve 1 and $P_1$ of curve 2 have to be equal
- $G_1$: $P_3 \rightarrow P_4$ of curve 1 and $P_1 \rightarrow P_4$ of curve 2 have to be on the same line
- $C_1$: $P_3 \rightarrow P_4$ and $P_1 \rightarrow P_4$ must be equidistant
- $C_2$ and above become difficult.

**Drawback of Bezier Curves**: Tangent continuity requries constraints linking control points. This essentially means that this representation requires the user to deal with ensuring the fact that their curves have the proper continuity. This leads us to consider another method for drawing curves.

### Cubic B-Spline

This spline method is easier to compute but the curve stays within the area between the 4 control points; there are no guarantees as to where within the 4 control points the curve lies. This basis is called the **B-Spline basis**. These curves guarantee $C_2$ continuity. An abstract way to think about is the difference between the Bezier and B-spline curves is that B-spline has a sliding window of control points while Bezier ones are sets of four points which are not intertwined.

**Converting between Bezier and B-Spline:** *refer to slides for more details. A lot of matrix math that you don't really need to memorize or anything.*

**Aside:** **NURBS (Non-Uniform Rational B-Spline) or Generalized B-Splines** are even more generalized concepts that exist. You can look this up online if interested.

### Representing Surfaces

Now that we have good tools at how we can represent curves, let's take a look at surfaces. There are several methods that are employed when we try to represent surfaces. The ones we study in this class are the following:

- **Triangle meshes**: This is essentially the same thing as using a collection of vertices to represent a curve. But instead of points, we have triangles
- **Tensor Product Splines**
- **Subdivision**

### From Curves to Surfaces

Say we have a Bezier curve parametrized by control points $P_{1-4}$. Then, we can have the control points be parametrized by some parameter $v$. I.e. $P_i(v)$. This will essentially create a **bicubic Bezier Surface**.

In order to create such a **control mesh** you need **16** control points. Computing this mesh given the control points requires us to do a tensor product for the master Bezier curve and the 4 control point Bezier curves. Using essentially the same type of analysis we did on curves, we can extend the techniques to surfaces.

**Note:** **Tangents and Normals for Patches** can be found by finding the partial derivatives for both dimensions for our plane and get a normal vector etc.

**Note:** **Tensor Product Spline Patches** would be the higher order/dimension equivalent of interpolating the points on our curve.

### More Cool Advanced Things 

**Displacement Mapping**: Essentially once you have your base surface you can add irregularities to the surfaces to give small detail.

**Subdivision Surfaces**: Idea is to subdivide a triangle and the triangle gets smoother with each iteration. Depending on the scheme we could be either using triangles or quads as our **Base for Subdivision**

**Triangle Subdivision**: Put more points at the middlepoint of each line segment.

*Fun fact(?): Triangle mesh where most vertices have **valence six**.*

## Lecture 4: Coordinates and Transofmrations *(aka Bookkeeping for Computer Graphcis)* (revised)

> The lecture today is [relatively] easier but your instructor is dislexic so this will somewhat difficult to follow(?) - Prof. Justin Solomon

### Hierarchical modeling

When we are dealing with graphical systems we have many coordinate systems:
- Camera
- Static scene
- car ... driver .. arm .. hand ... finger ... nail etc

*Philosophical question ripped off from 8.01: Where is the origin (0,0,0) to all this nonsense?*

In these coordinate systems we has many different objects:
- Points -- represent locations
- Vectors -- represent movement, force, displacement from A to B
- Normals -- represent orientation, unit length
- Coordinates -- numerical represerntation of the above objects **in a given coordinate system**

### Vectors

*The following will be a very quick crash course on 18.02 (tbh more along the lines of 18.022 and 18.100)*

**Matrices** are used to transform things or change coordinates (same thing different intuition).

A **vector space** is a set of elements equipped with addition and scalar multiplication. In a **vector space**

- There is a special element, the zero vector (no displacement, no force)
- We can use a basis to produce all the vectors in the space.
- Given a basis $B$ and constants $\textbf{c}$, we can get a vector $\vec\upsilon = B \textbf{c}$

**WARNING:** Matrices cannot translate points!!

The term **Linear operator** implies:

$\mathcal{L}(\vec v) = \mathcal{L} (\sum_i c_i \vec b_i) = \sum_i c_i \mathcal{L}(\vec b_i)$

Since $\mathcal{L}(\vec b_i)$ is a vector in the same space, it can be expressed in the $B$ basis. Therefore, we can express our linear operator as a matrix $M$ s.t.

$\mathcal{L}(\vec b_i) = \sum_j M_{ji} \vec b_j$

Going back to the original equation of defining $\textbf{c}$, we can see that a linear operator is doing the transformation

$B \textbf{c} \rightarrow B M \textbf{c}$

We can interpret this equation as the matrix $M$ being either applied to $B$ (meaning this is a change of basis or a change of coords) or $\textbf{c}$ which is transforming the point in the coords without chaning the coords system.

Let's say we have the basis $A = B M$. Then given the coordinate of $\vec v$ in $B$: $\vec v = B \textbf{c}$ when expressed in terms of the basis $A$ we have $\vec v = A M^{-1} \textbf{c}$.

### Points

*Okay so beyond this point, I couldn't keep up with noting down everything so I put in the main ideas with as much context as possible. This section is best understood if read through along with the lecture slides.*

We will denote points with a tilde on top of the variable name to distinguish them from vectors.

$\widetilde{p} = \widetilde{o} + \sum_i c_i \vec b_i := F\textbf{c}$

Points $\widetilde{p}$ are 4-dimensional column matrices.

Point-vector operations:
$\widetilde{p} - \widetilde{q} = \vec \upsilon$
$\widetilde{p} + \vec \upsilon = \widetilde{q}$

All these transformations are affine: translations and transforms preserve points, straight lines and planes. Now, we are applying an affine transform to point $\widetilde{p} = F \textbf{c}$. So basically, we are combining the translation component into the 3D transform matrix and making a 4D matrix. The translations in this new **Affine tranform matrix** is encoded in the 4th column.

**One important thing to notice** is that a vector will have the 4D column matrix representation with $(c_1, c_2, c_3, 0)$ while a point has the representation $(c_1, c_2, c_3, 1)$. **NOTICE THE 0 VS THE 1!**

**Similarity transformation** is a transformation that is intended to be used for a vector in a particular frame.

### Homogeneous coordinates

**Key Idea**: Cameras cannot perceive depth. Two different points could be seen as the same from a camera angle that is in line with both the points.

**Homogeneous coordinates** are all points of the form $(x,y;w)$ where we identify $(x,y;w) \equiv (cx,cy;cw)$. To get real coordinates from homogeneous coordinates we get $(x/w, y/w, w)$.

So, the idea is that $w$ is a measure of scale... how far something actually is. So if $(x,y;0)$ is a point at infinity.

*Refer to illustration on lecture slide for intuition for what $w$ means.*

## Lecture 5: Hierachical Modelling (Guest Lecture) 

*Guest lecturer ..... dude's favourite film is Transformers. I'm already upset... Maybe he only likes it cuz its has all the cool graphics shit, which leads me to believe that this dude doesn't actually care about film at all.*

Hierarchical modelling is a method for modelling complex objects by comibining with simple primitives into complex aggregates.

### Matrix Chaining

*Dude covers the basics of vector addition, multiplication etc for context on how vectors and coordinate systems would be used for setting up complex chain of relative frames of reference.*

*No offense but, the lecture continues to explain very basic linalg (dumbed down). No need for lecture notes so long as you were able to follow during Lecture 4.*

**Note:** When applying a rotation and translation matrix to a point, we should always translate first and rotate second as the new position and orientation would be calculated erroneously otherwise. When reversing a transformation, we do the operations in opposite order of course.

### Forward Kinematics

Seems like in this section, we're talking about how applying the transformation matrices to the left and stacking them up allows us to define points relative to a specific joint which could also be defined recursively to other joints. As such, we're able to express a point relative to a moving and rotating reference frame with little complexity.

**Joint State Parameters**

Each joint has some parameters characterizing it relative to its parent:
- Offset: where the origin of the joint is relative to its parent
- Orientation: what the orientation of the joint is relative to its parent
- Limit: how much this joint is able to rotate

$P_{world} = M_{0,1} M_{1,2} ... M_{n-1, n} P_{local}$

**Pros and cons of this model**

Pros: very efficient during computation
Cons: hard to account for how world object affect local objects. We solve this issue by using **Inverse Kinematics**.

### Inverse Kinematics

*Hoh, we now have some animation lingo kicking in here.*

**Inverse Kinematics** is the inverse problem of **Forward Kinematics**.

Unfortunately, this system is not possible to be solved analytically in most cases *which tbh makes a lot of sense*. Hence, we usually need numerical methods such as **Jacobian iterative method** and **Optimization based methods** *(lol probably means "read: SGD")*

**The Jacobian**

If $\textbf{e} = F(\theta)$, the **Jacobian** is defined as $J = d F(\theta) / d \theta$.

The **Jacobian** is used to guide the position of where local object should be relative to its parents by applying an optimization algorithm. See lecture slides for pseudocode for this algorithm.

*this is literally a variation on gradient descent where we're using a slightly complex structure like the Jacobian as a guide.*

### Hierarchical Tree and Scene Graph

When we're dealing with very simple objects like an arm, we simply need to keep an array of matrices that allow us to do the computations easily. However, once we have more than one child joint for every joint, we realize that we need a **hierarchical tree** to represent this hierarchical model. There is a pretty straightforward graphic that illustrates this idea.

In order to update the values of each joint, we simply need to do a DFS recursion and draw it.

If we're dealing with more general objects that are not necessarily hierarchical (the relation of child and parent doesn't necessarily apply), we should be using a graph instead of a tree. This allows us to refer to different instances of the same objects by applying different transformation matrices to the objects.

### Recap: GPU Rendering Pipeline

1. Input geometry + attributes
2. Vertex Processing
3. Primitive processing
4. Rasterization
5. Fragment processing
6. Pixel processing
7. Frame buffers.

In this process the memory is used extensively to store and read geometry, textures and buffers.

To learn about using OpenGL, refer to [LearnOpenGL](https://learnopengl.com/)

## Lecture 6: Basics of Computer Animation Skinning/Enveloping

**Nano quiz 2 notes**

- **Anisotropic scaling** means different scale factors on different dimentions. Therefore is a linear transformation.
- **Perspective transformation** is not a linear transformation. It's an affine transformation.

### Traditional Animation

*All animation was done frame by frame. Refer to **Animator's Survival Guide** and **The Illusion of Life** for more details on how everything worked back in the good old days.*

*Wow I'm recognizing animation technical lingo terms lol. I feel so legit knowing what all these words mean. モンデ的のポイント高い！*

Key animators (the important one) and the one that does the bitchwork (in-betweens). Multi-perspective panoramas painted (this was painted on a flat plate and panned across the canvas to emmulate 3D camera panning). Layers to put several objects across several different z-axis locations.

**Multiplanar Camera $\rightarrow$ Digital Cel Animation (CAPS)**

Disney moved on from the mechanical (multiplanar camera) which used physical distances to emulate the z-axis which was replaced by computers instead.

*Watching really early CGI... Confirmed early integration of traditional hand-drawn animation with CGI was fake as shit.*

**Idea**: The splines we learned are essentially going to our in-betweeners.

*Mention of squash stretch and a couple of animation terms.*

> "It's all about the spacing and timing" - quote from **Animator's Survival Guide**

The technical contraints of **Computer Animation: Film vs Video Games**: Film rendering time doesn't matter while in games the realtime is very important.

### Computer Graphics for Animation

Types of Animation:

- Keyframing: We need to specify the keyframes for the animation and the spline will give me the in-betweens.
- Procedural: We define rules of how the objects move algorithmically.
- Physically based: Emulate some physics.

### Animation Controls

Animation is usually specified using some form of low-dimensional controls as opossed to remodelling the actual geometry of each frame.

> Oooh, transitions [in the slides] - Prof. Justin Solomon

**Animation Pipeline**

- Art & Story
- Modelling
- Rigging
    By rigging how the joints are connected to each other we can do
    - Forward Kinematics in order to figure out where models have to be in world frame
    - Inverse Kinematics in order to figure out how the models are oriented given their world frame positions
- Skinning
    Embedding a skeleton into a detailed character mesh. We do this by binding the skin vertices to the bones so that we can animate the bones and have the character render the mesh according to the binding rules.
    
*Digression*: We can use motion capture and/or do **IK** in order to get what the skeleton should look like. This makes it easier to capture keyframes for the animation.

### Character Animation (Skinning/enveloping)

**What we know so far:** How to animate a bone hierarchy (Change the joint angles, i.e., bone transformations, over time (keyframing))

We will use **Skeletal subspace deformation** technique to skin our characters.

The idea is that every bone has a deformation of the space around it (rotation, translation). We need to make sure we don't attach each vertex of the skin to a single bone (otherwise, you'd stab yourself with the given vertex into your bone). Instead, we want to attach a vertex to many bones at once and parametrize the location of the vertex as a function of its distance from both bones.

I.e. places that don't experience any folding (your forehead, back of your hand etc) doesn't need more than one bone to be attached. When we get to closer to the joints, we need to attach to several bones to emulate folding.

**Vertex Weights**

We'll assign a weight $w_{ij}$ for each vertex $\textbf{p}_i$ for each bone $\textbf{B}_j$. This weight essentially says how much the point $\textbf{p}_i$ is affected by the bone $\textbf{B}_j$. We see that logically the sum of all weights for a point $\textbf{p}_i$ should be 1.

**How do we calculate vertex positions?**

### Linear Blend Skinning

**Basic Idea 1:** Transform each vertex $\textbf{p}_i$ with each bone as if it was tied to it rigidly.
**Basic Idea 2:** Then blend the results using the weights

Say we rotate our elbow. The skin that are closer to your forearm move together with the forearm and vice versa. At the joint though, it goes halfway between rotating and not. This allows for a nice and smooth transition. Unfortunately, **SSD** is not perfect. As this is just a few rules that dictate how vertices move relative to the bones and not an actual physical simulation of what actually should happen.

**The Bind Pose**

We are given a skeleton and a skin mesh in a default pose. This is called a bind pose. Here, we get the undeformed vertices $\textbf{p}_i$ which are given in the object space of the skin.

*Mayday! can't keep up with the slides*

**Note:** Imporant formula for transforming a skeleton transformation. Look at slides for formulas.

*After a lot of math we kinda got results on how to transform vertices mapped to the skeleton according to the skeleton transformation.*

## Lecture 7: Particle Systems and ODEs *(aka How to do Video Game Physics)*

In this lecture, we'll cover how physically based animations are implemented in computer graphics.

**Key Idea:** What your eyes perceive as real is neither real nor accurate to actual physics.

**Aside:** Continuum Mechanics (fluids in) 6.838.

### Physically-Based Animation

We computer graphics, **simulate physics** by solving equations of motion. These animations are realistic but unfortunately difficult to control.

**Types of Dynamics** we will consider in this course:
- Point
- Rigid body
- Deformable body (clothes, fluids, smoke)

### Point Dynamics or Particle Systems

Particle systems are basically lots of points interacting together. **Emitters** generate **"particles"**. The external **forces** from force field affect the particles. We **integrate** the laws of mechanics for motion. **Randomness** tends to create nice effects. More advanced behaviour of points could be to simulate crowds and flocks.

**Smoothed Particle Hydrodynamics (SPH)** (for fluid simulation)

### Let's make a sprinkler

```
PL: linked list of particle = empty
spread=0.1
color=Spread-0.1
for each tstep
    generate k particles
        p=particle
        PL.add(p)
    for each particle p in PL
        update position and velocity
        draw_particle(p)
```

### Ordinary Differential Equations

$\frac{d \vec x}{d t} = f ( \vec x(t), t)$

Newtonian mechanics can be mapped using the vector $(x,v)$ as your basis.

Look at lecture notes for more details on the ODEs formulation.

Point is, you don't want a high order ODE solver whenever you add more complex interactions in your physical simulation.

**Phase Space** is a higher dimensional representation of $x$ and $x'$.

### Integrating ODEs

Numerical solution is called "numerical integration" (related to "quadreture"). In our case, we simply choose some small $dt$ and apply the displacement. This gives us **piecewise-linear** paths. We immediately notice that the choice of $dt$ is crucial in the accuracy of the resulting paths. Hence, you want $dt$ to be small in order to get more accurate measurements. Unfortunately, this has the drawback of being computationally intensive.

### Advanced: Variational Integration

**Note:** We have explicit (artificial driving), implicit (artificial damping) and variational integrators.

**Force**: You have zoo of different forces: gravity with constant $g$, gravitational law, viscous damping, spatial fields

## Lecture 8: Particle Systems and ODE Solvers II, Mass-Spring Modelling

**Nanoquiz Notes**: Bind pose is **NOT** updated. The poses are created by transforming the current pose from the same shape as the bind pose.

### Cont'd from last lecture

Last time: We can solve ODEs used in physics, we can just use a first order using the $(x, v)$ basis (aka the **Phase Space**).

We can use the **Euler Method** which is basically a Riemman sum. The drawback of this method is that it's not always stable.

Let's consider the test equation:

$f(x,t) = -kx$ (Exponential Decay)

The solution to this ODE is

$x(t) = x_0 e^{-kt}$

If we were to use **Euler's Method** to numerrically solve this ODE, we get:

$x_{t+h} = x_t + h f(x_t, t)\\
x_{t+h} = x_t - hkx_t\\
x_{t+h} = (1-hk)x_t$

So if h is too large, this ends up becoming not a decay but an explosion *lel*.

**Note:** If $k$ is large $h$ must be small.

When there are such constraints to a problem, these problems are **numerically stiff**. I.e. diffeq for which numerical methods are unstable without tiny time steps. It's annoying to deal with such problems is that often you don't actually care about the high-frequency effects that break down your simulation.

**WARNING:** Accuracy and stability are not the same

### Accuracy Analysis: Taylor Series

If we do a Taylor Series analysis of the **Euler's Method** we'd see that the $h$ step used in the method has an error $O(h^2)$. This is a accurate to **first order**.

**Note:** look at lecture slides for why this is **first order**. But the main idea is, although the error decreases quadratically with $h$, you also have to compensate by multiplying the number of steps you take by $h$. Therefore, you end up with a linear accuracy improvement on $h$.

Let's now consider a different **Trapezoid Method**

The method is formalized as follows:

$f_0 = f(\textbf{X}_0, t_0)$
$f_1 = f(\textbf{X}_0+hf_0, t_0 +h)$

This will give us $O(h^3)$ second order accurate, stable on model problem but can oscillate. Details in the math was omitted as they are immaterial to the point.

**Midpoint Method** also gives us an approximation with $O(h^3)$ error.

**Idea**: We can actually get the more and more orders of accuracy at the expense of making the solution more unstable + be more computationally expensive.

In the graphics world, we use **Runge-Kutta 4 (RK4) Integrator**. This gives us a standard fourth-order explicit integrator. The idea of this method basically coincides with how the **Trapezoid Method** works.

The integrators we have studied up till now are **explicit integrators**.

### Backward/Implicit Euler

The key idea is that where **Forward Euler** computed $x_{i+1}$ by using $x_{i}$, we now do the opposite in **Backward Euler**. Applying this new method to our original problem of **Exponential decay** we get:

$x_t = x_{t+h} (1+hk)$ or

$x_{t+h} = \frac{x_t}{(1+hk)}$

We realize that from what the coefficient looks like that, the solution never explodes. I.e. it's **Unconditionally Stable**. Of course if we took large $h$ values, it will no longer be **accurate** but it will still be **stable** at least.

The **pros** for this method is that it's stable at the **cost** of more computational complexity.

### Mass-Spring Modelling

Cloth can be simulated as a grid of mass-spring system. Before we talk more more about this let's take a detour to how we simulate something simpler: string. We'd do this using particles strung together with springs.

> Phenomenonological engineering [with some difficulty nailing the pronunciation]

**Aside cool paper**: Bergou et a. Discrete Elastic Rods. SIGGRAPH 2008.

Okay, anyways, going back to cloth. There are three types of forces in cloth:

- Structural (enforce invariant properties of the system -- distance between two particles to be constant)
- Internal deformations (shapes)
- External (gravity)

We put springs as follows:
- Strucural springs `(i,j)` and `(i+1,j)`; `(i,j)` and `(i,j+1)`
- Deformation springs
    - Shear springs `(i,j)` and `(i+1, j+1)`; `(i,j)` and `(i,j-1)`
    - Flexion springs `(i,j)` and `(i+2,j)` and `(i,j)` and `(i,j+2)`

> If you have very loose springs, you'll have the Charlie Brown of the cloth world.

### Implementation notes

We have very nice abstractions to help us work through the graphics pipeline. The Particle System can have different implementations of how particles are created and have forces act on them. The Time Stepper can have different interpolation methods like forward, backward Euler's and RK4.

## Lecture 9: Ray Casting (revised)

So far in class we've covered about modelling different scenes and their behaviours whether it be through keyframe or physically based animations. However, we've not discussed how these scenes that we've defined up until now are supposed to be displayed as image frames (in our assignments 1-3, the assignment code provided to render the scenes but never explained how that worked in detail.) Today, we'll learn what **rendering** means and how to generate an image given the scene descriptions we've learned to implement up until now using basic **ray casting**!

### Rendering

**Rendering** refers to the entire process that produces a 2D image with colour values for its pixels given a 3D representation of a scene. Individual pixels correspond to rays that originate in our eyes, passes through the pixel location on the 2D image plane, and hits a visible point in the scene. In order to determine the colour for these pixels, we need to calculate how much **light** the point in the scene receives. This action is often referred to as **lighting/shading**.

There are two major types of algorithms that are used within the Computer Graphics community: **Ray Casting** and **Rasterization**. Both of these algorithms are applicable within the industry and have different use cases. In this course, we will cover both algorithms (**Ray Casting** for the next 3-4 lectures followed by **Rasterization** for another 2-3 lectures).

Today we will cover:
- Ray Casting Basics
- Camera and Ray Generation
- Ray-Plane Intersection
- Ray-Sphere Interesction

### Ray Casting

**Key Idea:** Our eyes perceive the images of scenes by sensing the **light intensity** on the surface of the objects in the scene. Ie the light emitted by the **light sources** bounces off the objects in the scene into our eyes. Conversely, we can think of this process as our eyes **casting a ray** into the scene until it **hits** an object and then determining what **light sources** are able to illuminate the point on the object in order to calculate the **shading** of the point.

**Note:** Rays from the camera(eye) to the object is called the **light ray** and the ray from object to the light source is called the **shadow ray** (more on this later).

**Shading** is a term used to describe how surfaces look. There are several things we need in order to figure out how a surface looks like, such as surface properties (normal vector, direction to light, viewpoint), material properties(texture, diffuseness/shininess), and light properties (light source locations, intensity and colour of the light sources).

In pseudo-code, the ray casting algorithm can be formalized as follows:

```python
for every pixel:
    construct a ray from the eye
    for every object in the scene:
        find the intersection with the ray
        keep if closest
    shade
```

**Aside:** **Ray Casting** is very **CPU** intensive while **rasterization** is very **GPU** intensive. This is mainly due to the `if` statement in the inner loop that prevents the instruction in the inner loop from being easily vectorizable. Although this could be worked around with, we will be looking at how this simple algorithm is extended in favour of improving the quality of the output image. These extensions will prevent us from keeping the atomic instructions from being simple enough for a GPU to be able to handle. Note that this extended version of the ray casting algorithm is called **Ray Tracing**.

### Ray Generation

We see that in the ray casting/tracing algorithm, we extensively use rays to help us find what parts of the scene to render and how to shade them. In order to use this **ray** abstraction, we want to have a good flexible mathematical representation of a ray that can be used to calculate **intersections** with different scene objects efficiently.

A ray using a pair of a **point** (the origin) and a **direction** (the vector). Given this representation we can represent a ray using the parametric line equation that represents the ray as

$P(t) = O + \vec{r}\ t$ for $t > 0$ where $O$ is the coordinates of the origin and $\vec{r}$ is the direction vector of the ray.

Now given this formulation, we also need to define a way to represent our camera(eye) and image canvas to be fully equipped to implement the ray casting/tracing algorithm.

### Image Coordinates

**Key Idea:** The human eye works by collecting incoming light, passing it through a tiny hole (the pupil) and projecting the light that passed through onto the back of the eyeball (high-school physics tells you that this image should be flipped upside down). In a sense, this tiny "pinhole" "selects" the light rays that allow us to perceive a coherent image. In our model, instead of recording the flipped upside down image that forms behind the camera origin, we can just put this imaginary screen in front of the camera.

Now, we've essentially defined an origin point for the camera as well as an image canvas some fixed distance in front of the origin. For convenience of calculations, we normalize our image canvas coordinates to range from -1 to 1 both in the $x$ and $y$ axes. In order to generate a ray that originates from the eye point $e$ and goes through the point $p$, we simply subtract the two points to get the ray's direction vector. Crunching the actual numbers requires us to know what the distance of the screen is from the eye point $e$ as well as the aspect ratio of the actual camera. This calculation is a simple matter of some basic vector algebra so the exact equations have been ommitted.

**Aside:** This projection scheme is called a **Perspective Projection** as there is a single eye collecting the image which produces images that show the effects of foreshortening (objects get smaller as they go farther from the camera). There is another projection scheme called an **Orthographic Projection** where instead of having a singular origin behind the image canvas and have different ray directions, we have every point on the image canvas be an origin for individual rays which all have the same ray direction (set manually by the user). Ie the rays are all parallel to each other and these images are more useful for giving us an accurate measure of proportions of 3D objects.

### Ray-Plane Intersection

At this point, we've equipped ourselves with the necessary tools for us to be able to calculate. Let's see how we would go about using these rays to detect intersections with different objects in the scene. The first one (and the simplest one) is a plane.

We represent a plane using the implicit plane equation. *This should be very basic vector algebra but if you need more details for the math involved in this section going forward, please refer to the lecture slides.*

$H(P) = ax + by + cz + d = 0$

For the purposes of what we do next, we want to have a signed distance from the plane. I.e. this is a faced plane. 

When we are looking at the ray $P(t)$ and looking at when the ray will hit the plane, we can calculate by writing $H(P(t)) = 0$ and solving for $t$. We should note that we make sure that the plane is not parallel with the plane in which case, it could be the case that the ray never intersects the plane.

**Note:** When we cast a ray, we assume that we only detect interections if they're in front of the screen (not between camera and screen or behind the camera). An easy way to make this check is to check if the interesction $t > t_{min}$ where $t_{min}$ is the parameter corresponding to the ray intersecting with the image plane.

### Ray-Sphere Intersection

Intersecting the ray with a sphere has essentially the same calculation with only the representation of the sphere being:

$H(P) = P^2 - r^2 = 0$ where $r$ is the radius of the sphere and $P$ is the coordinate of the sphere's origin point.

**Note:** We can move the point $P$ to the origin in the interest of simplicity and transform the ray when calculating the intersection.

Calculating $H(P(t)) = 0$, we see that we have to solve a quadratic equation. The two solutions are the two intersections of the ray with the sphere. The correct $t$ to choose is the smaller one (except if the smaller root is negative).

*We can also make some basic algebraic analysis on the conditions for intersection and which roots of the equation to pick but we have omitted it here.*

**Note:** Please take care when calculating normals since in some cases the camera might be inside the sphere. 

## Lecture 10: Ray Casting II (revised)

Cont'd from last lecture...

By now, we've figured out the basic approach to these problems. Find the surface equation for the object being intersected, substitute the $P(t)$ instead of the $(x,y,z)$ within the surface equation, and solve for $t$.

### Ray-Triangle Intersection

For triangles, we could use a ray-plane intersection followed by an in-triangle test. But a smarter approach is to use **barycentric coordinates**. These coordinates are essentially tuned to be used for interacting with points within a triangle.

**Barycentric coordinates** are defined for a (non-degenerate triangle) $(\textbf{a}, \textbf{b}, \textbf{c})$. Any point on this plane is written as

$P(\alpha \geq 0, \beta \geq 0, \gamma \geq 0) = \alpha \textbf{a} + \beta \textbf{b} + \gamma \textbf{c}$.

In order for $P$ to be within the triangle, $\alpha + \beta + \gamma = 1$ must be true, so there are only 2 degrees of freedom in this equation.

**Aside:** From the definition of the center of mass of three points, we see that the **barycenter** point $P$ is the center of mass if masses $\alpha$, $\beta$, $\gamma$ were positioned on the three corresponding vertices.

For an easier way to represent our point using these coordinates is to write this as:

$P(\beta, \gamma) = \textbf{a} + \beta \textbf{e}_1 + \gamma \textbf{e}_2$ where $\textbf{e}_1 = \textbf{b} - \textbf{a}$ and $\textbf{e}_2 = \textbf{c} - \textbf{a}$

Solving these constraints, we get to a **linear equation** we can solve for to get the values $\beta$ and $\gamma$.

Now, since we have represented the intersection point as $P(\beta, \gamma)$, we can equate this to our ray equation $P(t)$ so that we can solve for the time $t$ so we know when and where the ray hits the triangle.

Since this a linear 3D vector equation with three unknowns, we can expect to be able to formulate this in the form $\textbf{A} \textbf{x} = \textbf{b}$ where $\textbf{x} = (\beta, \gamma, t)^T$.

*Since the derivation of matrix $\textbf{A}$ and vector $\textbf{b}$ is fairly straightforward and tedious at worst to compute, this has been omitted from these notes. Please refer to lecture notes for full derivation and final formula.*

**Fun aside:** Since the total of $\alpha$, $\beta$ and $\gamma$ is 1, we can use this to smoothly interpolate colours (or even textures!) from vertex to vertex smoothly. 

### Stray Ideas

In this section, we will raise short discussions about some topics that we will briefly mention but now explore in great depth before moving forward. These ideas are important to know exist and have some familiarity in order to be follow along what comes afterwards.

**Ray Casting: Object Oriented Design:** So far, we've learned how to intersect different primitive objects with different rays. In general, we can have an object class called `Object3D` which are implemented by different objects s.t. `Plane`, `Sphere`, `Triangle Mesh` etc. and provide a layer of abstraction between the generalized rays and the different types of objects it intersects with. **Assignment 4** introduces us to how this is implemented using OOP.

**Constructive Solid Geometry (CSG):** You can make a complex shape using simple shapes and boolean algebra. In order to render these shapes, we want to be keeping track of what segments of the rays when the ray is inside an object. Then, we are essentially getting a 1D problem for each ray.

### Transformations in ray tracing

**Key Idea:** When have **multiple copies** of the **same object** in a scene, we do not want to be having multiple copies of it in memory. Instead, we simply want to have the object loaded once and have **multiple instances point** to the **same object**. Of course in that case, we'd want to have **different transformations** of the same object to be saved in these "instance objects". This is a valid option but admittedly not the simplest way to go about this. So, instead of transforming the objects, we simply **transform the ray** so that we can calculate the intersection points.

**Note:** When dealing with transforming normals, we must be careful how we transform them as a straight transform would give us an erroneous result.

*The reasoning as to why is somewhat difficult to explain without diagrams. Please refer to lecture slides have very good breakdown of how normals should be transformed in these situations. Alternatively you could google "transform surface normal vector" should give you good results.*

In summary, positions and directions should be transformed by the full homogeneous matrix $M$. (dismiss the translation component for directions). For normals, we apply $M^{-T}$ without the translation component.

## Lecture 11: Ray Tracing (revised)

So far, we've learned about casting a ray towards the scene and detecting basic information about the surface that intersects the ray (object id, intersection point and normal). Today, we will extend this model so that we can correctly render shadows, reflections and refractions using the **Ray Tracing** algorithm.

### Shadows

**Key Idea:** A shadow is the absense of light. If a light source is occluded by a different object, the light from the light source will not be able to illuminate the point on the object. The easiest way to do this check with the tools we have so far is to simply cast a ray from the point of the intersection towards the light and we'd be able to tell if the first thing the ray hits is the light source or not. This ray is called the **shadow ray**. Extending our ray casting pseudo-code from before we get:

```python
for every pixel:
    construct a ray from the eye
    for every object in the scene:
        find the intersection with the ray
        keep if closest
        
    colour = ambient light * diffuse colour
    for every light source:
        emit shadow ray from intersection point to light source
        for every object:
            find the intersection with the shadow ray
            if intersection exists before reaching light source:
                colour += shade for light source
                    
    pixel value = colour
```

**Note:** A note of caution on implementation of the **shadow ray** is that if floating point rounding gets messed up, the shadow ray might intersect with the original point and will end up in the object "casting a shadow on itself", or ...

> [You get the classic] elementary school problem of [being like] stop hitting yourself. -- Prof. Justin Solomon

### Reflection

**Key Idea:** Using a very similar thought process as used for calculating shadows is to instead of treating a mirror as a diffuse object, we will reflect the ray like you'd reflect on a real mirror and calculate the pixel value from the result of what the reflected ray decides to render. Our pseudo-code becomes

```python
for every pixel:
    construct a ray from the eye
    for every object in the scene:
        find the intersection with the ray
        keep if closest
        
    colour = ambient light * diffuse colour
        
    if object is specular:
        call trace ray as if the reflected ray was a pixel needed to render
        colour += trace ray output * specular colour
        
    for every light source:
        emit shadow ray from intersection point to light source
        for every object:
            find the intersection with the shadow ray
            if intersection exists before reaching light source:
                colour += shade for light source
                    
    pixel value = colour
```

**Note:** In order to calculate $\textbf{R}$ (the reflected ray) given vectors $\textbf{V}$ (the incident ray), and the $\textbf{N}$ (the normal to the surface) is

$\textbf{R} = \textbf{V} - 2 (\textbf{V} \textbf{N}) \textbf{N}$.

**Aside:** In reality, perfect reflection doesn't exist all too often. There's more complex ways to model light to make reflection look more realistic using the **Fresnel reflection term**. We will discuss about a way of implementing this later in the course.

### Refraction

**Key Idea:** Same as reflection, we simply cast a ray in the refracted direction according to **Snell's law**. If $\textbf{T}$ is the transmitted light, $-\textbf{I}$ is the incident light and $\textbf{N}$ is the normal vector:

$\textbf{T} = [\eta_r (\textbf{N} \textbf{I}) - \sqrt{1 - \eta_r^2 (1 - (\textbf{N} \textbf{I})^2)}] \textbf{N} - \eta_r \textbf{I}$

*The derivation of this equation is quite long and requires understanding of **Snell's Law**. This is more related to Physics (Optics in particular) rather than Computer Graphics so I've omitted the details of the derivation in these notes. For more details on the derivation for this part either refer to the lecture slides of google **Snell's Law** and try deriving the above equation. Read: if you care enough, deriving this would probably consolidate your understanding of how this works.*

### More extensions

**Key Idea:** The Ray Tracing algorithm is very easily extensible to emulate a lot of cool effects. In this section we'll mention very briefly some of these cool tricks.

**Reflective/refractive materials:** We want to average over several rays with slightly perturbed normal vectors to emulate the slightly distorted look.

**Antialiasing -- Supersampling:** Instead of taking the center of each pixel and sending a ray from there, we want to send several rays from different points within the pixels and average the values together to avoid bad artifacts (more on this in a later lecture).

**Motion Blur:** Sample temporally and get a blurry object.

**DOF:** Apply blur for far objects to emulate how camera's work.

## Lecture 12: Performance considerations for the Ray Tracing Algorithm (revised)

So far we've studied how the ray casting/tracing algorithms are used to render a scene. In the ray tracing algorithm in particular, not only do we cast rays for each pixel, but potentially many rays per pixel (if we're super-sampling for whatever purpose) as well as recursive calls to support reflective/refractive materials. Ie the algorithmic complexity of this algorithm blows up the more realistic we want to make our images.

Therefore, it's very important to try and optimize this algorithm as much as possible. Let's quickly take a look at what the ray tracing algorithm looks like right now:

```python
trace ray
    intersect all objects
    color = ambient term
    for every light:
        cast shadow ray
        color += local shading term
    if mirror:
        color += color_refl * trace reflected ray
    if transparent:
        color += color_trans * trace refracted ray
```

> Everytime I'm bored in a restaurant I try to use chopsticks to point out why the painter's algorithm fails. -- Prof. Justin Solomon

### Conservative Bounding Volume

**Key Idea:** When we calculate whether a ray hits a complicated object in the scene, we would regularly check if the ray hits **each** of the triangles that compose the object. This is a **VERY** expensive operation. Instead, we could simply put a *bounding region* around our object and if the ray doesn't hit the box, the ray definitely doesn't hit the object. This way, we avoid having to iterate through each triangle of the complex object and only do a check if the ray hits the bounding box which is a much cheaper operation.

Let's consider the simple case of an **axis-aligned bounding box**. Constructing the bounding box is quite easy as we just take the min and max values in each axis and construct 6 sides of the box. We can also treat these in the sense we did during the CSG lecture so it's computationally faster to find when the ray intersects the bounding box.

### Bounding Volume Hierarchies (BVH)

In this current scheme, if a ray hits a bounding volume, we test all primities inside it. This is lots of work. So instead, we'd have hierarchies of bounding volumes.

The question then becomes "Where to Split Objects?"  We could do it at the 
- Midpoint fo the current volume
- Sort and put half of the objects on each side
- Use modelling hierarchy

These are all valid options and depending on the implementations different schemes are used.

**Pros** of a **BVH** are that they are easy to construct and traverse
The **cons** are that they are difficult to choose a good split for a node

### Kd-trees

A very popular **BVH** within the computer graphics community is the kd-tree. The data structure is set up as follows:

```c++
KdTreeNode {
    KdTreeNode* frontNode, backNode; // children
    int dimSplit; // either x, y, or z
    float splitDistance; // from origin along split axis
    bool isLeaf;
    vector<Triangle> triangles; // only for leaves
};
```

Using this data structure, we'll look at how we can optimize our algorithm for finding ray intersections with objects in our scene.

### Kd-tree Traversal

In a kd-tree node, a ray will intersect the boundaries at least twice (the outer boundaries) at $t_{start}$ $t_{end}$ respectively, and potentially intersect the middle divider at $t$. The pseudo-code for finding an intersection using kd-trees is as follows:

```python
traverse(orig, dir, t_start, t_end):
    if self.isLeaf:
        return intersect(self.listOfTriangles, orig, dir, t_start, t_end)
    t = (self.splitDist - orig[self.dimSplit]) / dir[self.dimSplit];
    if t <= t_start:
        return self.backSideNode.traverse(orig, dir, t_start, t_end)
    elif t >= t_end:
        return self.frontSideNode.traverse(orig, dir, t_start, t_end)
    else:
        t_hit = self.frontSidenOde.traverse(orig, dir, t_start, t)
        if t_hit <= t: return t_hit;
        return self.backSideNOde.traverse(orig, dir, t, t_end)
```

In terms of how we actually build these trees with a good distribution of objects, we'd need to come up with a good heuristic for determining a good place to split. One good heuristic is the **Surface Area Heuristic**: We want a high probability that we need to intersect a child given that we intersected a leaf node.

*This rabbit-hole goes pretty deep. so yeah. Read at your own discretion.*

## Lecture 13: Shading & Material Appearance (revised)

Taking a step back from performance of the algorithm, this lecture we're revisiting how we could make our ray traced images look more realistic and look at material textures more in depth.

### Lighting and Material Appearance

When we're trying to render a realistic image we'd need its geometry, material and how the scene is lit. The material's appearance depends on the intensity and shape of the highlights, the glossiness, colour, and spatial variation (texture).

**Note:** This course is not a physics course. There will be many gross oversimplifications on how we model the underlying physical phenomena that make real things look real. There will be points where units of measurement will break down somewhat. For a more in-depth breakdown of how to study these phenomena more rigorously you want to look at **radiometry**.

> People's hair is very interesting. It's not really an issue for me -- Prof. Justin Solomon

**Note:** In this lecture, we only consider point light sources (no need to care about solid angles and integrals). For multiple light sources we simply use linearity (additive and multiplicative intensities) of light.

### Light Intensity

**Note:** Light intensity falls off for isotropic point lights with order $1/r^2$. But in graphics we often cheat/ignore this. In graphics, a popular fall-off term is $1/(ar^2+b^r+c)$ as ideal point lights are quite harsh and we add the linear and constant term to deal with that corner case.

We use the **light intensity** received at a point in the scene in order to calculate what colour the point on the object should look like when rendered. The amount of light energy received by a surface depends on incoming angle with the $\cos \theta$ law (read: dot product with normal).

> Will this surface get lit? -- Prof. Justin Solomon

Putting both of these ideas together, we get an idea for how to calculate the intensity of light a surface bounces off towards the camera.

$I_{in} = I_{light} \cos \theta / r^2$

To briefly discuss about some different types of light sources:

**Directional lights** are point lights that are essentially infinitely far so there shouldn't be any fall-off in intensity with distance:

$I_{in} = I_{light} \cos \theta$ (notice how the units are already inconsistent for $I$)

**Spotlights** are point lights with non-uniform directional emission. Usually they are symmetric about a central direction $d$, with angular falloff. The formula for these lights would vary depending on implementation but the core idea is fairly straightforward.

### Quantifying Reflection: BRDF (Bidirectional Reflectance Distribution Function)

In our previous discussions with reflective materials, we've only briefly mentioned that ideal mirrors are not encountered too often and there are a lot of materials that have non-ideal reflectivity. In order to model such surfaces, we define the BRDF (Function).

**BRDF:** The Bidirectional Reflectance Distribution function tells us the ratio of light coming from one direction that gets reflected in another direction. In pure reflection, we assume no light scatters into the material and it is all reflected symmetrically around the normal.

**BRDF** is a function $f_r(\theta_i, \phi_i; \theta_o, \phi_o)$ or $f_r(\textbf{l}, \textbf{v})$.

*Look at lecture slides for good illustration of these angles.*

The **BRDF** $f_r$ relates the incident irradiance from every direction to outgoing light:

$I_{out}(\textbf{v}) = I_{in}(\textbf{l}) f_r(\textbf{v}, \textbf{l})$.

From the number of degrees of freedom of this function we can also point out the definition of a commonly used term in the computer graphics community:

**Isotropic vs Anisotropic**: When keeping $\textbf{l}$ and $\textbf{v}$ fixed, if rotation of surface around the normal does not change the reflection, the material is called isotropic. Surfaces with strongly oriented microgeometry elements are anisotropic. E.g. brushed metals, fair, fur, cloth, velvet.

In practice, the **BRDF** of materials are parametrized by creating a mathematical expression to represent the function. There would be some basic attributes that could be tuned using the parameters such as "shininess", "anisotropy" etc.

**Aside:** One other way of obtaining the **BRDF** of a material is to use a **Gonioreflectometre** to calculate the **BRDF** experimentally. Another tool is to take pictures of spheres coated with the material and rotate light around a 1D arc. This provides us 3DOF (degrees of freedom) which means we can use this for isotropic materials only (anisotropic materials require use of 4DOF).

Now we will look at a couple of different **BRDF** functions that are widely used and that could be put together in conjunction to get a very wide variety of different possible surfaces looks.

### Ideal Diffuse Reflectance

The ideal diffuse **BRDF** is a constant $f_r = const$. These are textures for chalk, clay and some paints: on a microscopic level these materials are very rough and thus don't reflect light in any particular direction. **Lambertian shading** we covered when we first covered the ray tracing algorithm would assume that the surface had **Ideal Diffuse Reflectance**.

The constant in the **BRDF** is $\rho / \pi$ where $\rho$ is the albedo. This coefficient is between 0 and 1 and says what fraction of the incoming light is reflected. In practice, this is usually just called the "diffuse colour" $k_d$.

### Ideal Specular Reflectance

Reflection only happens at mirror angles. The **BRDF** for an ideal mirror is a Dirac Delta function (read: it kills the intensity in every direction but the reflection angle). Note that light **only** reflects to the mirror direction. So we take the Dirac delta multiplied by a specular coefficient $k_s$. *(Look up Dirac delta if you're interested about what this means.)*

### Non-ideal Reflectors

Combining the **BRDF**s we've looked at above, we can come up with surfaces that have some diffuseness as well as shininess. In assignment 4, we will be implementing the **Phong Specular Model** to shade our surfaces. So, we'll explain how this particular model works.

In the **Phong Specular Model**, we take the light that is reflected times the $k_s (\cos \alpha)^q$ where $k_s$ is the specular reflection coefficient and $q$ is the specular reflection exponent. This is termed as the **Specular Lobe**. The shape of this lobe is $(\textbf{v} \textbf{r})^q$.

In practice, we shade the surface by the reflective component, the diffuse component, and the ambient lighting. The ambient lighting is a total hack(!) but this also allows us to avoid the complexity of indirect ("global") illumination.

Putting the whole thing together we has formula:

$L_o = [k_a + k_d (\textbf{n} \textbf{l}) k_s (\textbf{v} \textbf{r})^q] \frac{L_i}{r^2}$

**Note:** This is not physically based. It doesn't even conserve energy.

Another shader model is the **Blinn-Torrance Variation** of **Phong**. Refer to lecture slides for more details. Aaaaand another one: **Cook Torrance Lobe**. Aaaaand another one: **Designer BRDFs**.

## Lecture 14: Texture Mapping & Shaders (revised)

All the materials we've seen so far have been the same everywhere on the object. For more interesting objects, we need some spatial variation on the colour. We want allow BRDF parameters to vary over space in order to give much more complex surface appearances. E.g. vary the diffuse colour $k_d$ with $\textbf{x}$. There are two main methodologies to go about this:

- From data: texture mapping. Colour and other information is loaded from 2D images.
- Procedural: shader. Little programs that compute info as a function of location.

> Rhinoeserous?... Hippo -- Prof. Justin Solomon

**Key Idea:** Texture maps are essentially like stitching wallpaper to 3D objects.

In order to stitch the wallpaper correctly, we create a correspondence between the mesh and 2D texture map so that whenever a ray hits a point on the 3D mesh, we'd be able to find the corresponding pixel on the texture map to shade.

### UV Coordinates

Each vertex P stores 2D(u,v) texture coordinates. These UVs deteremine the 2D location in the texture for the vertex and then interpolate using barycentric coordinates. This technique is called **texture lookup**. Unfortunately this technique kinda sucks. This is due to the fact that depending on the scale that we see the shape, we are likely to get a lot of noise upon lookup of the $(u,v)$ coordinates. The solution:

**MIP Map:** Multum in Parvo (much in little). The idea of a **MIP map** is to resample the texture map before rendering begins and stores all of them for lookup later.

### Calculating UV Coordinates

Per-vertex (u,v) "texture coordinates" are specified:
- Manually, provided by user (tedious!)
- Automatically using parametrization optimization
- Mathematical mapping (independent of vertices)

**Texture UV optimization**'s goal is to "flatten" 3D objects onto 2D UV coordinates. For each vertex, we find coordinates u,v such that distortion is minimized.

**Note:** Texture maps don't have to have be about just the colour. It could be the different $k$s or the normal vectors.

### Procedural Textures

**Procedural Textures** are an alternative to texture maps. A little program computes the colours as a function of $f(x,y,z)$. These are easy to implement but is hard to intuit. Good example of a cool looking procedural textures using **Perlin Noise**.

**Perlin noise** is an example of a very versatile procedural texture generator commonly used in the graphics community. To briefly describe this random noies generator: given a domain of $[0,\infty]$, we will set the value of 0 at integer locations. Then, we add pseudo-random derivatives (1D gradient) and then interpolate a curve given the control points and derivatives. In 2D, we basically do bicubic interpolation. This gives us smooth noise that doesn't diverge far from the origin.

## Lecture 15: Sampling, Aliasing, Mipmaps (revised)

> This is a little bit of an experimental lecture. -- Prof. Justin Solomon

In today's lecture we will discuss about common issues that arise with ray traced images and how we combat them. Out strategy today is to **employ mathematical and perceptual tricks** to sample our images in order to improve them.

**Key Idea:** Philosophically speaking, the physical world is continuous while software is discrete. Graphics is about translating a continuous problem into a discrete one.

**What is a pixel?** It's not a box, nor a disk nor a tiny light. A pixel looks different on different display devices. A pixel is a sample: it has no dimension, occupies no area, it cannot be seen, it has a coordinate and a value.

**Sampling** mapping a continuous function to a discrete one.
**Quantiziation** mapping a continuous variable and mapping to a discrete one. As IEEE 32bit double percision floating point arithmetic is quite well defined so we'll ignore the effects of **quantization** as we wouldn't be able to detect them in our use cases.

### Sampling and Reconstruction

Before discussing the issues that come with sampling and reconstruction, let's look at how the process works within our pipeline.

0) Visual light is a continuous function
1) Sample it (with a digital camera or ray tracer; gives a finite set of numbers: discrete)
2) Reconstruct a continuous function (for example the point spread of a pixel on a CRT or LCD)

Both the steps here can create problems like
- pre-aliasing caused by sampling
- post-aliasing caused by reconstruction

These aliasing issues mostly arise due to our choice of the **sampling density**. Insufficient sampling makes high frequencies look like low frequencies ("aliasing"): hiding in a high frequency wave is a low frequency wave having chosen the appropriate sampling rate.

**Aside:** the term aliasing comes from the fact that the new low-frequency wave doesn't actually exist... it's kind of like a ghost/alias.

The solution to get rid of aliasing is to either **blur** or **oversample** the image. For audio, we put a low-pass filter, for images we apply a blur.

### Math (but not really to understand what's going on)

Our model rendering in 1D would be to sample the function $y = f(x)$. I.e. given the inputs $f(x_i)$ for some steps $x_i$ we want to see if we can reconstruct the function $f(x)$. The fancy word for this is to multiply the function by an **impulse train** (sum of displaced Dirac delta functions).

Depending on the step size of the **impulse signal**, we will lose all the information with higher frequencies than our step size... enter the **Fourier Transform**.

**Fourier Theorem(ish)**: Any function can be written as a combination of sines and cosines.

What the **Fourier Transform** does is that it tries to represent how much of wiggles of different frequencies exist in the signal. Given a function in its **Fourier domain**, we can eliminate terms of different frequencies to get different filters of the signals called **Low pass**, **Band pass** and **High pass** filters.

To convert a function to its **Fourier basis**, we need to get the projection of the function in the **Fourier basis** (think of it as a dot product between the basis function and the source signal).

**Nyquist rate**: The lowest alias-free sample rate; two times the bandiwdth of a band-limited signal

**COOL IMPORTANT NOTE**: Multiplication in Fourier Domain is a convolution in Primal Domain.

**Note**: Taking the inverse Fourier Transform of a box gives us a $sinc$ function.

This set-up where we simply reconstruct our image with a well chosen $sinc$ reconstruction seems a little bit too good to be true. And indeed, **Practical signals cannot have finite bandwidth.**

### In practice: Supersampling

Uniform supersampling gives us the high frequencies and we're able to downsample using a good filter. However, we still get frequencies above the limit to be aliased. To combat this, we instead we take a uniform sample + random perturbation. The signal processing becomes more complicated, but we will avoid the aliasing.

## Lecture 16: Global Illumination and Monte Carlo (revised)

All the ray traced images we've rendered so far are super sharp... they look ideal. Today we will address this issue with *lots of randomness*.

Contents:
- Global Illumination
    - Rendering Equation
    - Path tracing
- Monte Carlo integration
- Better sampling
    - importance
    - stratification

### Global Illumination

So far, we've only seen **direct lighting**. However, we also want **indirect lighting**. We want the full integral of the light coming from all directions (multiplied by **BRDF**). In practice, that's not computable so instead, we try to approximate this result by sending tons of random rays and taking the average. In **global illumination**, instead of only calculating the **BRDF** for the ray to the light sources, we want to instead integrate over all directions.

$L_{out}(x, \textbf{v}) = \int_\Omega L_{in}(\textbf{l}) f_r(x, \textbf{l}, \textbf{v}) \cos \theta dl + E_{out}(x, \textbf{v})$

This is called the Rendering equation. The issue with this equation is the fact that $L_{in}$ is the $L_{out}$ of the previous bounce. This is called an "integral equation" with the unknown solution function $L$ both on the LHS and the RHS, inside the integral. We must either discretize these equations or use **Monte Carlo integration** in order to solve them. Read: An analytic solution to this is usually impossible. There are some ways of solving it approximitely which is what we will cover in this class.

### Monte-Carlo Ray Tracing

Everytime we send a ray, we instead send more than one ray in different directions. And instead of recursing many times, we only recurse once. To avoid the noisy output we send a lot of secondary rays but much less tertiary rays.

*This part of the lecture is quite difficult to describe with notes so instead please refer to the slides. The demo photos show good results of what's happening with each different feature.*

This version of indirect lighting ends up making the images too smooth. Ironically, the more computation we spend, the less sharp the image ends up being.

### Irradiance Caching

As we just saw above, the effects of indirect lighting is very smooth. Therefore, it makes sense for us to sample indirect lighting effects sparsely and interpolate them to save on computation. Please note that for direct illumination, we should still do full calculation as the direct lighting term gives us the sharpness of the image.

### Photon Mapping

This technique could be even further improved by implementing **Photon Mapping**. This method requires us to pre-process the scene by emitting rays in random directions from all the light sources and bounce around and record the photons that "stick" to surfaces in the scene. Then, when we render the scene, we can use these **photon map** values to compute the indirect illumination effects through **irradiance caching**.

### Monte Carlo Integration

In our rendering pipeline depending on what we're trying to achieve, we end up stacking a lot of integrals. Integrating with uniform sampling gives makes our algorithm to be prone to aliasing. Instead we want to do **Monte Carlo Integration**. Although this integration method is a lot slower with convergence rate of $O(1/N^2)$, the integral is not dependent on any scale. This allows us to avoid any aliasing.

## Lecture 17: Graphics Pipeline & Rasterization (revised)

So far we've discussed extensively about the ray tracing algorithm. Although the results of this algorithm are quite impressive, the biggest **issue** this algorithm has is the **runtime**. If we want to produce any ray traced image, it'd take a couple of seconds to render. However, when we need to be pumping out 30 fps animation on screen, the ray casting algorithm does not fair too well.

Here, we're not optimizing for making something that looks good as fast as possible. But, we want to make as much work done **GIVEN** we have a constant amount of time.

The process of taking a triangle and figuring out which pixels it covers is **rasterization**. We can accelerate rasterization using different tricks than ray tracing. (We're not actually going to test all pixels for each triangle).

Given a triangle's vertices, we can figure out which pixels to turn on. Then, we want to compute illumination values to fill in pixels within the primitive. At each pixel, we need to keep trac of the closest primitive (z-buffer). We will only overwrite if triangle being drawn is closer than the previous triangle in that pixel.

### Differences between Ray Tracing and Rasterization

In the basic form, **ray tracing** needs the entire scene descriptor at once. Meanwhile, in **rasterization**, we can simply draw an object in the scene and the discard it. Therefore, we won't need to keep the object in memory around anymore.

Unfortunately, the limitation of **rasterization** is that you're mostly restricted to triangles to use as primitives since SGD doesn't really work on GPUs. Faceting and shading artifacts are difficult.

### Modern Graphics Pipeline

Let's look at how **rasterization** fits into the graphics pipeline step by step.

- Input
    - Geometric model -- triangle vertices, vertex normals, texture coordinates
    - Lighting/material model (shader) -- light source positions, colours, intensities etc., texture maps specular/diffuce coefficients
    - Viewpoint + projection
- Output
    - Pixels

1. Project vertices to 2D

**Idea** we want to remap the **view frustum** into a rectangle. This is called the canonical view volume.

*Look at slides for transformation matrix for calculating the **OpenGL Form of the Projection**.*

2. Rasterize triangle: find which pixels should be lit

The primitives are "continuous" geometric objects; screen is discrete (pixels). Rasterization computes a discrete approximation in terms of pixels. The way we determine whether a pixel is inside a triangle or not is to just take three inequalities for the three planar lines that define the triangle. This can be done in the GPU cuz boolean algebra fiy.

We can also rasterize hierarchically to make the process faster.

3. Compute per-pixel colour

As mentioned when we were doing ray tracing, we can invoke the shader functions for texture mapping to get the shading done for each rendered triangle. Therefore, most of the discussion that we had on texture mapping applies for the discussion of pixel colouring here as well.

4. Test visibility, update frame buffer

More on this next lecture.

The pseudo-code for this pipeline looks as follows:

```python
for each triangle:
    transform into eye space (perform projection)
    setup 3 edge equations
    for each pixel x,y:
        if passes all edge equations:
            compute z
            if z < zbuffer[x,y]
            zbuffer[x,y] = z
            framebuffer[x,y] = shade
```

## Lecture 18: Graphics Pipeline & Rasterization II (revised)

**Cont'd from last time**

> Determine whether each pixel is lit... Is this some millenial thing cuz every time i use [the word lit you get this reaction] -- Prof. Justin Solomon

4. Test visibility, update frame buffer

In order to detect whether a point on an object should be in front or behind another point is to keep track of the depth of both points. So, in addition to frame buffer (R,G,B) we also store the distance to camera (z-buffer). The pixel is updated only if the *newz* is closer than the current z-buffer value.

### Interpolation in Screen Space

So, how do we get the Z value at each pixel? We only know z at vertices... We must interpolate from vertices into the triangle interior.

Let's say $P$ is a point in $R^3$. The projection matrix $\textbf{C}$ projects the point $P$ onto screen space. Then we see that:

$\textbf{C} P = \textbf{C} (\alpha \textbf{a} + \beta \textbf{b} + \gamma \textbf{c})$

However, after this multiplication by $\textbf{C}$, in order to convert these homogenous coordinates to screen coordinates we have to divide by the $w$ dimension of the pint $\textbf{C} P$.

**Note:** This division causes the transformation from texture space to screen space no longer be a simple linear transformation as it now involves a division.

### Multisampling vs. Supersampling

We've figured out the correct way to find how to interpolate our textures onto the screen. And similarly to how we had problems with anti-aliasing etc., we still have these issues in rasterization. We could solve the issue using the same technique as in ray tracing: supersampling. However, this idea has some issues as requiring too high a supersampling factor to have a good effect. Instead, what we could to is to instead of fully render a more high resolution image and blur afterwards, we could simply supersample the visibility and compute the expensive shading only once per pixel. This saves us a lot of computation.

### Texture Filtering

In order to avoid bad texture effects such as moire patterns, we want to filter our textures similarly to how we did for ray tracing. This time, to determine how textures that are far away should be sampled, we simply need to figure out how a low-pass filter that is applied on the screen translates to the surface of the object in the scene. This can be done simply with the use of the **Jacobian** of the blur function, but in practice, we'd simply calculate the size of the filter in world space and find the corresponding MIP map entry for sampling.

## Lecture 19: Real-Time Shadows

**Shadows** in the **rasterization** pipeline is very **difficult** in contrast to how easy it was to implement within the Ray tracing algorithm. Remember that in the ray tracing algorithm, we simply send a secondary ray towards the light source and detect whether an obstruction exists in the way.

Today, we will talk about shadow maps and shadow volumes which are methods for rendering shadows within the **rasterization** pipeline.

### Shadow Maps

**Key Idea:** A point being illuminated is equivalent to saying that a point is visible from the light source. This means that we could simply "rasterize" an image from the perspective of the light source and use that information to see which points in the scene should be lit.

In our pipeline, this is implemented by doing two passes to render the image. ... TODO

### Shadow Map Aliasing

Unfortunately, since we've used rasterization to create the shadow-map, we've not only introduced aliasing to the image, but also the shadow map.

**Key Idea:** Filtering the shadow map doesn't really make sense since discontinuities within the depth map must remain sharp. Instead then, we can sample perturbed points in the scenes using the aliased shadow maps and take the average. This way, although the shadow map is still aliased, the final immage is not as jagged while not introducing weird effects.

### Cascading Shadow Maps

**Key Idea:** Similarly to how we used MIP maps for rendering textures, we have TODO

### Shadow Volumes

**Stencil Buffer**.


