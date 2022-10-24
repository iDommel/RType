# Message Format:

### legend:
1. *--* signifies a viariable number of arguments depending on the value of previous argument

---

`messageType --`

## *messageType* values {#entityMessage-values}:

1. ### *entityMessage* syntax {#entityMessage-syntax}:
*entityMessage* = 0 : A message that describes an action by the server on an entity,
  to be reproduced by the client

`entityMessage action entityID --`

### *action* values {#action-values}:

*created* = 0 : request to create a new entity of given ID

`entityMessage created entityID entityType --`



*updated* = 1 : request to update an existing entity of specified ID

`entityMessage created entityID entityType --`

*deleted* = 2 : request to delete an existing entity of specified ID

`entityMessage created entityID`

### *entityType* values {#entityType-values}:


---

2. ### *graphicEventMessage* syntax {#graphicEventMessage-syntax}:
*graphicEventMessage* = 1 : A message a graphic (raylib) event received by the client,
  to be transmitted to the server

``

- *networkEventMessage* = 2: A message related to the connection between the client and the server

---


