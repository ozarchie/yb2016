# yb2016

This code is re-crafted from yieldbuddy -    

https://github.com/yieldbuddy

I could not figure out how to merge the arduino and RaspberryPi portions of this site for a single repository, so this one was created.

The code has all the fixes suggested at -  
http://yieldbuddy.com/vanilla/discussion/1576/17a-fresh-installation-and-the-start-of-a-user-installation-manual  
  and a few more.

There are a few extra updates, that remove most of the web errors,  
and stops nginx filling up the disk by turning off error reporting.


Motion is still problematic -  
localhost:8080/8081 work, but not the image on the summary.  
The current motion.conf does not save images, fluttering leaves fill up the disk!


