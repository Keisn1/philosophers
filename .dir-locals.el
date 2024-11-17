;;; Directory Local Variables
;;; For more information see (info "(emacs) Directory Variables")

((org-mode . ((eval . (add-hook 'after-save-hook
								(lambda nil
								  (when
									  (string-equal
									   (file-name-nondirectory buffer-file-name)
									   "README.org")
									(org-pandoc-export-to-gfm)))
								nil t))))

 (nil . ((org-roam-directory . "/home/kay/workspace/philosophers/roam")
		 (org-roam-db-location . "/home/kay/workspace/philosophers/roam.db")
		 )))
