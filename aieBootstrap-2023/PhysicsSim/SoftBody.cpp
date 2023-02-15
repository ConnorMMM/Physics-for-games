#include "SoftBody.h"

#include "PhysicsScene.h"
#include "Circle.h"
#include "Spring.h"

/// <summary>
/// Builds a softbody with the provided information.
/// </summary>
/// <param name="scene">: The Physics scene that the softbody is to be created for </param>
/// <param name="position">: The starting position the softbody is built from </param>
/// <param name="damping">:  </param>
/// <param name="springForce">: </param>
/// <param name="spacing">: </param>
/// <param name="strings">: </param>
void SoftBody::Build(PhysicsScene* scene, glm::vec2 position, float damping,
	float springForce, float spacing, std::vector<std::string>& strings)
{
	int numColumns = strings.size();
	int numRows = strings[0].length();

	// traverse across the array and add balls where the ascii art says they should be
	Circle** circles = new Circle * [numRows * numColumns];
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numColumns; j++)
		{
			if (strings[j][i] == '0')
			{
				circles[i * numColumns + j] = new Circle(position + glm::vec2(i, j) * spacing, glm::vec2(0, 0), 1.0f, 2.0f, glm::vec4(1, 0, 0, 1));
				scene->AddActor(circles[i * numColumns + j]);
			}
			else
			{
				circles[i * numColumns + j] = nullptr;
			}
		}
	}

	// second pass - add springs in
	for (int i = 1; i < numRows; i++)
	{
		for (int j = 1; j < numColumns; j++)
		{
			Circle* s11 = circles[i * numColumns + j];
			Circle* s01 = circles[(i - 1) * numColumns + j];
			Circle* s10 = circles[i * numColumns + j - 1];
			Circle* s00 = circles[(i - 1) * numColumns + j - 1];

			// make springs to cardinal neighbours
			if (s11 && s01)
				scene->AddActor(new Spring(s11, s01, damping, springForce, spacing));
			if (s11 && s10)
				scene->AddActor(new Spring(s11, s10, damping, springForce, spacing));
			if (s10 && s00)
				scene->AddActor(new Spring(s10, s00, damping, springForce, spacing));
			if (s01 && s00)
				scene->AddActor(new Spring(s01, s00, damping, springForce, spacing));

			if (s11 && s00)
				scene->AddActor(new Spring(s11, s00, damping, springForce, spacing * sqrt(2.0f)));
			if (s01 && s10)
				scene->AddActor(new Spring(s01, s10, damping, springForce, spacing * sqrt(2.0f)));

			bool endOfJ = j == numColumns - 1;
			bool endOfI = i == numRows - 1;

			Circle* s22 = (!endOfI && !endOfJ) ? circles[(i + 1) * numColumns + (j + 1)] : nullptr;
			Circle* s02 = !endOfJ ? circles[(i - 1) * numColumns + (j + 1)] : nullptr;
			Circle* s20 = !endOfI ? circles[(i + 1) * numColumns + j - 1] : nullptr;

			if (s00 && s02)
				scene->AddActor(new Spring(s00, s02, damping, springForce, spacing * 2.0f));
			if (s22 && s20)
				scene->AddActor(new Spring(s22, s20, damping, springForce, spacing * 2.0f));
		}
	}
}