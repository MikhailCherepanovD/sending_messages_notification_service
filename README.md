# Sending message service
Sending message service is part of [Flight ticket tracking application](https://github.com/MikhailCherepanovD/notification_service).

## Functionality

This service in an endless loop checks the table allocated to it in the database for routes, information about which needs to be updated (the interval set by the user has passed).

If the routes for updating are found, the server sends a GET request to Notification_Service along the path /users/{user_id}/routes/{route_id}/current. Notification_Services updates the information in the database. After that, ticket information is sent to subscribers in Apache Kafka.
