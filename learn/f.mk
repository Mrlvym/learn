.PHONY:all
all:
	@echo hello world  
	@echo $(A)

	cd /home;pwd
	pwd
	
define two_line
echo haode
echo $(A)
endef

B:=$(join 12345,abcd)

all1:
#	@echo $(two_line)
	@echo $(B)

