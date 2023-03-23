.PHONY: asmp-dll asmp-server asmp-exe-patcher test build test clean help

asmp-dll:
	$(MAKE) -C asmp-dll $(filter-out $@,$(MAKECMDGOALS))

asmp-server:
	$(MAKE) -C asmp-server $(filter-out $@,$(MAKECMDGOALS))

asmp-exe-patcher:
	$(MAKE) -C asmp-exe-patcher $(filter-out $@,$(MAKECMDGOALS))

test:
	$(MAKE) -C test $(filter-out $@,$(MAKECMDGOALS))

all:
	$(MAKE) -C asmp-dll $(filter-out $@,$(MAKECMDGOALS))
	$(MAKE) -C asmp-server $(filter-out $@,$(MAKECMDGOALS))
	$(MAKE) -C asmp-exe-patcher $(filter-out $@,$(MAKECMDGOALS))
	$(MAKE) -C test $(filter-out $@,$(MAKECMDGOALS))

help:
	@echo "This is the global Makefile for all components of the project."
	@echo "Usage: make [COMPONENT] [TARGET] [VARIABLES]"
	@echo ""
	@echo "Components:"
	@echo "  asmp-dll         - client (asmp.dll)"
	@echo "  asmp-server      - server (asmp-server.exe)"
	@echo "  asmp-exe-patcher - game executable patcher (asmp-exe-patcher.exe)"
	@echo "  test             - tests"
	@echo "  all              - all components"
	@echo ""
	@echo "Targets:"
	@echo "  build - build a component of the project"
	@echo "  clean - remove data created during component building"
	@echo "  help  - "
	@echo ""
	@echo "Variables:"
	@echo "  Avaliable variables are component-specific. Call 'help' for"
	@echo "  a specific component for more information."
