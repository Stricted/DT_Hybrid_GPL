import sys
import requests
import json
import os
import stat
import os.path

#urlbase = "http://10.167.100.178:3000"
urlbase = "http://rnd-ams.huawei.com"

payload = {"customer":sys.argv[5], "vendor":sys.argv[6]}
headers = {'content-Type': 'application/json; charset=UTF-8'}

packages = []

def load_all_packages():
	global packages
	r = requests.get(urlbase + "/sub_modules.json")
	packages = json.loads(r.text)

def find_package_id(file_name):
	global packages
	for pack in packages:
		if -1 != pack[0].find(file_name):
			return pack[1]
	return None

# image
# bin
# lib
# html
# etc
# cms
# kernel
# bootloader

def get_file_type(file_path):
	if file_path.endswith(".ko"):
		return "ko"
	if file_path.startswith(os.path.join(sys.argv[1], "lib")) or file_path.startswith(os.path.join(sys.argv[1], "usr/lib")):
		return "lib"
	if file_path.startswith(os.path.join(sys.argv[1], "bin")) or file_path.startswith(os.path.join(sys.argv[1], "sbin")) or file_path.startswith(os.path.join(sys.argv[1], "usr/bin")) or file_path.startswith(os.path.join(sys.argv[1], "usr/sbin")):
		return "bin"
	if file_path.startswith(os.path.join(sys.argv[1], "html")):
		return "html"
	if file_path.endswith("webidx") or file_path.endswith("webimg"):
		return "html"
	return "etc"

def get_total_of_type(items, type):
	size = 0
	for item in items:
		if item["category"] == type:
			size += item["size"]
	return size

print(os.getcwd())
print(sys.argv[1])
print(sys.argv[2])
print(sys.argv[3])
print(sys.argv[4])
print(sys.argv[5])

items = []

# First load all packages
load_all_packages()

# Add items in target directory
for top, dirs, files in os.walk(sys.argv[1]):
    for nm in files:
    	file_path = os.path.join(top, nm)
    	if not os.path.islink(file_path):
    		if -1 == file_path.find("/dev/"):
    			items.append({"title":os.path.basename(nm), "size":os.path.getsize(file_path), "category":get_file_type(file_path), "sub_module_id":find_package_id(os.path.basename(nm))})
        		#print file_path + " -> " + get_file_type(file_path) + " -> " + str(os.path.getsize(file_path))

# Add cms items
for top, dirs, files in os.walk(os.path.join(sys.argv[2], "lib")):
	for nm in files:
		file_path = os.path.join(top, nm)
		basename = os.path.basename(nm)
		if basename.startswith("lib") and basename.endswith("cms.a"):
			items.append({"title":basename, "size":os.path.getsize(file_path), "category":"cms", "sub_module_id":find_package_id(basename)})

img_dir = os.path.join(sys.argv[1], "../images")

# Add image item
if os.path.exists(os.path.join(img_dir, sys.argv[3])):
	#items.append({"title":sys.argv[3], "size":os.path.getsize(os.path.join(img_dir, sys.argv[3])), "category":"image", "sub_module_id": None})
	payload["size"] = os.path.getsize(os.path.join(img_dir, sys.argv[3]))

# Add kernel item
if os.path.exists(os.path.join(img_dir, sys.argv[4])):
	payload["kernel"] = os.path.getsize(os.path.join(img_dir, sys.argv[4]))

# Add bootloader item
if os.path.exists(os.path.join(img_dir, "cferom.bin")):
	payload["bootloader"] = os.path.getsize(os.path.join(img_dir, "cferom.bin"))

# Get total size of all types
payload["bin"] = get_total_of_type(items, "bin")
payload["lib"] = get_total_of_type(items, "lib")
payload["etc"] = get_total_of_type(items, "etc")
payload["html"] = get_total_of_type(items, "html")
payload["ko"] = get_total_of_type(items, "ko")

payload["svnlog"] = ""
if len(sys.argv) > 7:
	payload["svnlog"] = sys.argv[7]
payload["items"] = items

r = requests.post(urlbase + "/products/addtag", data=json.dumps(payload), headers=headers)
#print(r.text)
