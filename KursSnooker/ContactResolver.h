#pragma once
#include <vector>
#include "Contact.h"

class Contact;

class ContactResolver
{
public:
	ContactResolver(unsigned int iterations, float velocityEpsilon = 0.01f, float positionEpsilon = 0.01f);
	ContactResolver(unsigned int velocityIterations, unsigned int positionIterations, float velocityEpsilon = 0.01f, float positionEpsilon = 0.01f);
	bool IsValid();
	void SetIterations(unsigned int velocityIterations, unsigned int positionIterations);
	void SetIterations(unsigned int iterations);
	void SetEpsilon(float velocityEpsilon, float positionEpsilon);
	void ResolveContacts(std::vector<Contact>& contactArray, unsigned int numContacts, float duration);
	unsigned int velocityIterationsUsed;
	unsigned int positionIterationsUsed;
protected:
	unsigned int velocityIterations;
	unsigned int positionIterations;
	float velocityEpsilon;
	float positionEpsilon;
	void prepareContacts(std::vector<Contact>& contactArray, unsigned int numContacts, float duration);
	void adjustVelocities(std::vector<Contact>& contactArray, unsigned int numContacts, float duration);
	void adjustPositions(std::vector<Contact>& contactArray, unsigned int numContacts, float duration);
private:
	bool validSettings;
};

