# Replication strategy
    Primary + Replica
    Get and Set for now both will be performed on primary, replica is for fault tolerance

# Flow for replica
    req from client-> primary node -> replica client -> replica node
    We are using repliction just for data recovery