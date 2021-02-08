# OpenSeekMap
Search efficiently for places in an [OpenStreetMap](https://www.openstreetmap.org) database, tolerating typos up to a certain level.

## About
The task was to be able to release certain queries on a dataset and then use approximate string matching algorithms such as the edit distance or shift-AND algorithm to find the best 5 matches in the dataset.

You can find the full assignment in Dutch [here](assignment/README.md). This was written as a project for the course Algorithms and Data structures 3 at the [University of Ghent](https://ugent.be).

## How to use
```
### WELCOME TO OPENSEEKMAP ###
Usage: ./openseekmap <database file> [longitude] [latitude] [--algorithm id] < <query>
Possible algorithms:
- 0. edit distance [default]
- 1. shiftAND with errors
- 2. shiftAND with errors legacy
```