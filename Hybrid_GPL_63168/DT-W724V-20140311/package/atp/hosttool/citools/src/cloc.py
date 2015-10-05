import sys
import os
import stat
import os.path
import commands

if not os.path.exists(sys.argv[4]):
	os.system(sys.argv[1] + " " + sys.argv[2])
	sys.exit(0)

result = commands.getstatusoutput(sys.argv[1] + " " + sys.argv[2] + " --quiet --csv --exclude-lang=make")

code = 0
comment = 0
for line in result[1].split("\n"):
	line = line.strip()
	if "" == line:
		continue
	if line.startswith("files"):
		continue
	attrs = line.split(",")
	code += int(attrs[4])
	comment += int(attrs[3])

with open(sys.argv[4], "a") as f:
	f.write("<item><name>%s</name><code>%d</code><comment>%d</comment></item>\n" % (sys.argv[3], code, comment))
