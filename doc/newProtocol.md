# Message Format:

### legend:
1. *--* signifies a viariable number of arguments depending on the value of previous argument

---

`messageType --`

## *messageType* values {#messageType-values}:

1. ### *entityMessage* syntax {#entityMessage-syntax}:
*entityMessage* = 0 : A message that describes an action by the server on an entity,
  to be reproduced by the client

`entityMessage action entityID --`

### *action* values {#action-values}:

*created* = 0 : request to create a new entity of given ID

`entityMessage created entityID entityType --`

refer to [entityType values](#entityType-values) for the values

*updated* = 1 : request to update an existing entity of specified ID

`entityMessage updated entityID entityType --`

refer to [entityType values](#entityType-values) for the values


*deleted* = 2 : request to delete an existing entity of specified ID

`entityMessage deleted entityID`

### *entityType* values {#entityType-values}:

*player* = 0: a player entity

`entityMessage updated entityID player position`
- *position* the new position of the player to be updated

`entityMessage created entityID player`

*enemy* = 1: an enemy entity

`entityMessage updated entityID enemy position`
- *position* the new position of the enemy to be updated

`entityMessage created entityID enemy enemyType position`
- *enemyType* has yet to be defined but will be used by the factory on the clientSide to generate an appropriate entity
- *position* position of the enemy to be created

*projectile* = 2: a projectile entity

`entityMessage created entityID projectile projectileType position`
- *projectileType* has yet to be defined but will be used by the factory on the clientSide to generate an appropriate projectile
- *position* position of the projectile to be created

`entityMessage updated entityID projectile position`
- *position* the new position of the projectile to be updated

---

2. ### *graphicEventMessage* syntax {#graphicEventMessage-syntax}:
*graphicEventMessage* = 1 : A message a graphic (raylib) event received by the client, to be transmitted to the server

`graphicEventMessage eventType --`

### *eventType* values:

*keyboardEvent* = 0: a keyboard event

`graphicEventMessage keyboardEvent key state`
- *key* the key that was pressed as described in the raylib.h header
- *state* the state of the previous key as described in the raylib.h header

*mouseEvent* = 1: a mouse event

`graphicEventMessage mouseEvent key state pos`
- *key* the key that was pressed as described in the raylib.h header
- *state* the state of the previous key as described in the raylib.h header
- *pos* the (x, y) position the mouse was at when the event occured


2. ### *networkEventMessage* syntax {#networkEventMessage-syntax}:
*networkEventMessage* = 2: A message related to the connection between the client and the server

---


