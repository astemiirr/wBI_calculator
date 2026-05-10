import tkinter as tk


from tkinter import filedialog, messagebox, scrolledtext, ttk
from threading import Thread
from pathlib import Path
from settings import RunSettings
from pipeline import Pipeline
from translator import translator

import paths


class MainWindow(tk.Tk):
    def __init__(self):
        super().__init__()

        self.tr = translator()

        self.t = self.tr.t

        self.title(self.t("title"))

        self.geometry("900x600")
        self.minsize(800, 500)

        self.pipeline = Pipeline()

        self.cpp_var = tk.StringVar(value=str(paths.find_wbi_cpp()))
        self.network_xlsx_var = tk.StringVar()
        self.quotas_xlsx_var = tk.StringVar()
        self.output_dir_var = tk.StringVar()
        self.k_var = tk.StringVar(value="2")
        self.mode_var = tk.StringVar(value=self.t("mode_parallel"))
        self.metric_var = tk.StringVar(value=self.t("metric_both"))

        self.mode_to_code = {
            self.t("mode_linear"): "linear",
            self.t("mode_parallel"): "parallel",
            self.t("mode_compare"): "compare",
        }

        self.metric_to_code = {
            "WBI 1": "wbi1",
            "WBI 2": "wbi2",
            self.t("metric_both"): "both",
        }

        self.build_ui()

    def build_ui(self):
        main_frame = ttk.Frame(self, padding=10)
        main_frame.pack(fill=tk.BOTH, expand=True)

        main_frame.columnconfigure(1, weight=1)

        # self.add_file_row(
        #     parent=main_frame,
        #     row=0,
        #     label="C++ exe:",
        #     var=self.cpp_var,
        #     save=False,
        # )

        self.add_file_row(
            parent=main_frame,
            row=0,
            label=self.t("quotas_file"),
            var=self.quotas_xlsx_var,
            folder=False,
        )

        self.add_file_row(
            parent=main_frame,
            row=1,
            label=self.t("network_file"),
            var=self.network_xlsx_var,
            folder=False,
        )

        self.add_file_row(
            parent=main_frame,
            row=2,
            label=self.t("output_folder"),
            var=self.output_dir_var,
            folder=True,
        )

        ttk.Label(main_frame, text="K:").grid(row=3, column=0, sticky=tk.W, pady=4)
        ttk.Entry(main_frame, textvariable=self.k_var, width=10).grid(
            row=3,
            column=1,
            sticky=tk.W,
            pady=4,
        )

        ttk.Label(main_frame, text=self.t("mode")).grid(
            row=4, column=0, sticky=tk.W, pady=4
        )
        ttk.Combobox(
            main_frame,
            textvariable=self.mode_var,
            values=[
                self.t("mode_linear"),
                self.t("mode_parallel"),
                self.t("mode_compare"),
            ],
            state="readonly",
            width=20,
        ).grid(row=4, column=1, sticky=tk.W, pady=4)

        ttk.Label(main_frame, text=self.t("metric")).grid(
            row=5, column=0, sticky=tk.W, pady=4
        )
        ttk.Combobox(
            main_frame,
            textvariable=self.metric_var,
            values=["WBI 1", "WBI 2", self.t("metric_both")],
            state="readonly",
            width=20,
        ).grid(row=5, column=1, sticky=tk.W, pady=4)

        self.start_button = ttk.Button(
            main_frame,
            text=self.t("start"),
            command=self.start_click,
        )

        self.start_button.grid(row=6, column=1, sticky=tk.W, pady=10)

        ttk.Label(main_frame, text=self.t("log")).grid(
            row=7, column=0, sticky=tk.NW, pady=4
        )

        self.log_text = scrolledtext.ScrolledText(main_frame, height=18)
        self.log_text.grid(row=7, column=1, sticky=tk.NSEW, pady=4)

        main_frame.rowconfigure(8, weight=1)

    def add_file_row(
        self, parent: ttk.Frame, row: int, label: str, var: tk.StringVar, folder: bool
    ):
        ttk.Label(parent, text=label).grid(row=row, column=0, sticky=tk.W, pady=4)

        ttk.Entry(parent, textvariable=var).grid(
            row=row,
            column=1,
            sticky=tk.EW,
            pady=4,
            padx=5,
        )

        ttk.Button(
            parent,
            text=self.t("select"),
            command=lambda: self.browse_file(var, folder),
        ).grid(row=row, column=2, pady=4)

    def browse_file(self, var: tk.StringVar, folder: bool):
        if folder:
            filename = filedialog.askdirectory(title=self.t("choose_folder"))
        else:
            filename = filedialog.askopenfilename(
                title=self.t("choose_file"),
                filetypes=[
                    ("All supported files", "*.xlsx *.exe"),
                    ("Excel files", "*.xlsx"),
                    ("Executable files", "*.exe"),
                    ("All files", "*.*"),
                ],
            )

        if filename:
            var.set(filename)

    def start_click(self):
        try:
            settings = self.read_settings()
        except ValueError as error:
            messagebox.showerror(self.t("input_error"), str(error))
            return

        self.start_button.config(state=tk.DISABLED)

        self.log_text.delete("1.0", tk.END)

        thread = Thread(
            target=self.run_pipeline,
            args=(settings,),
            daemon=True,
        )

        thread.start()

    def read_settings(self) -> RunSettings:
        if not self.quotas_xlsx_var.get():
            raise ValueError(self.t("choose_quotas"))

        if not self.network_xlsx_var.get():
            raise ValueError(self.t("choose_network"))

        if not self.output_dir_var.get():
            raise ValueError(self.t("choose_output_folder"))

        try:
            k = int(self.k_var.get())
        except ValueError:
            raise ValueError(self.t("k_must_be_an_integer"))

        if k <= 0:
            raise ValueError(self.t("k_must_be_positive"))

        return RunSettings(
            cpp=Path(self.cpp_var.get()),
            quotas_xlsx=Path(self.quotas_xlsx_var.get()),
            network_xlsx=Path(self.network_xlsx_var.get()),
            output_xlsx=Path(self.output_dir_var.get()) / "result.xlsx",
            k=k,
            mode=self.mode_to_code[self.mode_var.get()],
            metric=self.metric_to_code[self.metric_var.get()],
        )

    def run_pipeline(self, settings: RunSettings):
        try:
            self.pipeline.run(settings, self._log_thread)
            self.after(
                0, lambda: messagebox.showinfo(self.t("done"), self.t("done_message"))
            )
        except Exception as er:
            error_text = str(er)
            self.after(0, lambda: messagebox.showerror(self.t("error"), error_text))
        finally:
            self.after(0, lambda: self.start_button.config(state=tk.NORMAL))

    def _log_thread(self, message: str):
        self.after(0, lambda: self._log(message))

    def _log(self, message: str):
        self.log_text.insert(tk.END, message + "\n")
        self.log_text.see(tk.END)
