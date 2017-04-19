function FrmAlert(what) {
	return !confirm(what);
}

function ResolveTieDialog(name0, name1) {
	let result = prompt(`Who goes first? Enter 0 for ${name0} or 1 for ${name1}`);
	if (result === "0") {
		return 0;
	}
	if (result === "1") {
		return 1;
	}
	return undefined;
}

function ErrFatalDisplayIf(condition, text) {
	if (!condition) {
		return;
	}
	alert(text);
}

function ErrNonFatalDisplayIf(condition, text) {
	if (!condition) {
		return;
	}
	alert(text);
}

export {
	FrmAlert,
	ResolveTieDialog,
	ErrFatalDisplayIf,
	ErrNonFatalDisplayIf
};