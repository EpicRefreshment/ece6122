/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 10/7/2025
Date Last Modified: 10/17/2025

Description:

Lab 3

This is a modified version of the controls.hpp file found from the ogl tutorials repo.
Only addition is the light switch toggle function prototype.

*/

#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs();
int lightSwitchToggle();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif