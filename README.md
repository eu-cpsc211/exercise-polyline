In-class exercise for dynamic memory allocation.

Notes on the `Polyline` class:

- It tracks a list of 2D points using the `Vector2` data type.
- It uses a dynamically allocated array of `Vector2` to store the coordinates of each point. The pointer to the array is stored in `m_points`.
- It supports drawing lines that connect each of the points.

1. Implement the member function `AddPoint()`.
2. Implement the destructor for `Polyline`.

Once completed, you should be able to click the left mouse button to add new points and extend the polyline. Click on the first point of the polyline to end it and start a new one. Click the right mouse button to clear all lines and start over.

Further exercise: refactor the `Polyline` class to use a `vector<Vector2>` instead of managing a dynamic array. Compare the two approaches.
