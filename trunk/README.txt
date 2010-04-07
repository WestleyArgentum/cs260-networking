Hi. Here are, in no particular order, things you should know about our project.

-We take a while to load in files. We will load them in, we will send them, but be very very patient.
-We do not open up a UDP port until we need to, and we shut it down in between jobs.
-Jobs are ... jobs that are passed to a file transfer thread who's job it is to update the jobs...
-We have a progress bar above the box where you enter text. When you are receiving a file it will display percent.
-While we read in data it is difficult to converse with others in chat... this is because we did not thread out the
loading, but we were under the impression that the main point of the assignment was to get file sending working so it
shouldn't matter anyway.
-To quit the server type quit into the command line... or just press the x.
-Lay out your UserInfo.txt file exactly as the one provided (though obviously you can change the values).
-You should use unique usernames.
-Do not send a file to yourself... I think it will get very confused. Identical socket /  ip pair.
-To run everything just hit play in visual studio 2008 (enter a port for the server and have the clients text file
set up).

Co-Creators:
Westley Hennigh
Brandon Traphagen