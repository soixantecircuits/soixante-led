soixante-led

Controlling LED through fadeCandy, using openFrameworks and ofxTimeline

# Install

Install [openFrameworks](openframeworks.cc)

Clone this repo inside path/to/of/apps/soixante

and then 

```
bash clone_addons.sh
make
make run
```

# Run fadecandy server

```
cd
git clone https://github.com/scanlime/fadecandy.git
cd fadecandy/server
make submodules
make
sudo ./fcserver path/to/of/apps/soixante/soixante-led/fade-candy-settings.json
open http://127.0.0.1:7890

```
